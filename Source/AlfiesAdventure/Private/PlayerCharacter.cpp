/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#include "PlayerCharacter.h"

#include "AbilitySystemComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"

#include "../AlfiesAdventureGameModeBase.h"
#include "BaseAttributeSet.h"

 // Sets default values for this character's properties
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = false;

	// Don't turn the pawn with camera movement
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Initialize collision
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
	GetCapsuleComponent()->SetCapsuleRadius(25.0f);

	// Setup camera springarm boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetMesh());
	CameraBoom->TargetArmLength = 600.0f;
	// Orient camera to control rotation (actor rotation, not mesh rotation)
	CameraBoom->bUsePawnControlRotation = true;

	// Setup follow cam
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	// Orient camera to control rotation (actor rotation, not mesh rotation)
	FollowCamera->bUsePawnControlRotation = true;

	// Setup basic character movement settings
	// Rotate mesh and movement towards current actor rotation when moving
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 840.0f, 0.0f);
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->JumpZVelocity = 480.0f;
	GetCharacterMovement()->AirControl = 0.2f;
	GetCharacterMovement()->GravityScale = 1.0f;

	// Create ability system and attribute set references
	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("Ability System"));
	AttributeSet = CreateDefaultSubobject<UBaseAttributeSet>(TEXT("Attributes"));
}

/**
* Main Update function called every frame
*
* @param DeltaTime time since last update / frame in seconds
*/
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsRocketHovering)
	{
		FVector LaunchDirection = GetActorUpVector();

		LaunchCharacter(LaunchDirection * DeltaTime * 1500, false, false);
	}
}

/**
* Getter for the ability system component. Necessary for implementation of the IAbilitySystemInterface class
*/
UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Attribute Accessors
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

float APlayerCharacter::GetHealth()
{
	return AttributeSet->GetHealth();
}

float APlayerCharacter::GetMaxHealth()
{
	return AttributeSet->GetMaxHealth();
}

float APlayerCharacter::GetEnergy()
{
	return AttributeSet->GetEnergy();
}

float APlayerCharacter::GetMaxEnergy()
{
	return AttributeSet->GetMaxEnergy();
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	DefaultMeshTransform = GetMesh()->GetRelativeTransform();

	AbilitySystem->RefreshAbilityActorInfo();
	AttributeSet->OnHealthChange.AddDynamic(this, &APlayerCharacter::OnHealthChange);

	this->GetCharacterMovement()->GravityScale = 1.0f;

	bIsRagdollEnabled = false;

	bIsRocketHovering = false;

	// Assign default abilities
	for (TMap<EAbilitySlots, TSubclassOf<UGameplayAbility>>::TIterator it = DefaultAbilities.CreateIterator(); it; ++it)
	{
		GrantAbility(it->Value, it->Key);
	}

	for (TSubclassOf<UGameplayEffect> EffectClass : InitializationEffects)
	{
		UClass* ResolvedEffectClass = EffectClass.Get();
		UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, ResolvedEffectClass);
		AbilitySystem->ApplyGameplayEffectToSelf(Effect, 1.f, FGameplayEffectContextHandle());
	}
}

void APlayerCharacter::EnableRagdollMode()
{
	if (!bIsRagdollEnabled)
	{
		GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
		GetMesh()->SetSimulatePhysics(true);

		AAlfiesAdventureGameModeBase* GameMode = (AAlfiesAdventureGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->DisablePlayerInput();

		GetCharacterMovement()->DisableMovement();
		bIsRagdollEnabled = true;
	}
}

void APlayerCharacter::DisableRagdollMode()
{
	if (bIsRagdollEnabled)
	{
		GetMesh()->SetSimulatePhysics(false);
		GetMesh()->SetCollisionProfileName(TEXT("CharacterMesh"));

		AAlfiesAdventureGameModeBase* GameMode = (AAlfiesAdventureGameModeBase*)GetWorld()->GetAuthGameMode();
		GameMode->EnablePlayerInput();

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

		SetActorLocation(GetMesh()->GetComponentLocation());
		GetMesh()->SetRelativeTransform(DefaultMeshTransform);
		bIsRagdollEnabled = false;
	}
}

void APlayerCharacter::TryActivateAbilityBySlot(EAbilitySlots Slot)
{
	if (AbilitySlots.Contains(Slot))
	{
		AbilitySystem->TryActivateAbility(AbilitySlots[Slot], true);
	}
}

void APlayerCharacter::CancelAbilityBySlot(EAbilitySlots Slot)
{
	if (AbilitySlots.Contains(Slot))
	{
		AbilitySystem->CancelAbilityHandle(AbilitySlots[Slot]);
	}
}

void APlayerCharacter::GrantAbility(TSubclassOf<UGameplayAbility> GrantedAbility, EAbilitySlots Slot)
{
	if (AbilitySystem)
	{
		if (HasAuthority() && GrantedAbility)
		{
			FGameplayAbilitySpecHandle AbilitySpecHandle = AbilitySystem->GiveAbility(FGameplayAbilitySpec(GrantedAbility, 1));
			AbilitySlots.Add(Slot, AbilitySpecHandle);
		}
		AbilitySystem->RefreshAbilityActorInfo();
	}
}

void APlayerCharacter::RevokeAbility(EAbilitySlots Slot)
{
	if (AbilitySlots.Contains(Slot))
	{
		// Clear the ability first, then remove it from the slot-map
		FGameplayAbilitySpecHandle RevokedAbilityHandle = AbilitySlots[Slot];
		AbilitySystem->ClearAbility(RevokedAbilityHandle);
		AbilitySlots.Remove(Slot);
		AbilitySystem->RefreshAbilityActorInfo();
	}
}

void APlayerCharacter::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	CancelAbilityBySlot(EAbilitySlots::AS_ROCKET_HOVER);

	AbilitySystem->RemoveActiveGameplayEffect(ActiveFallingEffect);

	UClass* ResolvedEffectClass = FallingEffect.Get();
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, LandingEffect);
	ActiveFallingEffect = AbilitySystem->ApplyGameplayEffectToSelf(Effect, 1.f, FGameplayEffectContextHandle());
}

void APlayerCharacter::Falling()
{
	Super::Falling();

	UClass* ResolvedEffectClass = FallingEffect.Get();
	UGameplayEffect* Effect = NewObject<UGameplayEffect>(this, ResolvedEffectClass);
	ActiveFallingEffect = AbilitySystem->ApplyGameplayEffectToSelf(Effect, 1.f, FGameplayEffectContextHandle());
}

void APlayerCharacter::OnHealthChange(float CurValue, float MaxValue)
{
	// Check for death and enable ragdolls in case
	if (CurValue <= 0)
	{
		EnableRagdollMode();
	}
}

