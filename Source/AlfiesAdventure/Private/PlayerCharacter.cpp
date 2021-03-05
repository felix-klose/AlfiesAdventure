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

#include "BaseAttributeSet.h"

 // Sets default values for this character's properties
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Don't turn the pawn with camera movement
	bUseControllerRotationYaw = false;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;

	// Initialize collision
	GetCapsuleComponent()->SetCapsuleHalfHeight(90.0f);
	GetCapsuleComponent()->SetCapsuleRadius(25.0f);

	// Setup camera springarm boom
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
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

}

/**
* Called to bind functionality to input
*/
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlayerCharacter::SetForwardMovementInput);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::SetRightMovementInput);

	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookRight", this, &APawn::AddControllerYawInput);
}

/**
* Getter for the ability system component. Necessary for implementation of the IAbilitySystemInterface class
*/
UAbilitySystemComponent* APlayerCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

/**
* Called when the game starts or when spawned
*/
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

}

/**
* Calculates the current forward directions and calls character movement based on current input
*/
void APlayerCharacter::SetForwardMovementInput(float Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	AddMovementInput(Direction, Value);
}

/**
* Calculates the current right directions and calls character movement based on current input
*/
void APlayerCharacter::SetRightMovementInput(float Value)
{
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0.0f, Rotation.Yaw, 0.0f);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Direction, Value);
}

