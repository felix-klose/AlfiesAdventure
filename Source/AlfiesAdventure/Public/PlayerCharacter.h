/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#pragma once

#include "CoreMinimal.h"

#include "AbilitySystemInterface.h"
#include "GameFramework/Character.h"

 // Include classes used in templates and hope for the best in regards to circular dependencies...
#include "GameplayAbilitySpec.h"
#include "GameplayEffect.h"

#include "PlayerCharacter.generated.h"

/** Accessor enum for Ability slots, used to make granting and revoking abilities for certain slots easier */
UENUM()
enum class EAbilitySlots : uint8
{
	AS_ROCKET_HOVER		UMETA(DisplayName = "Rocket Hover"),
};

UCLASS()
class ALFIESADVENTURE_API APlayerCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:

	/** Player follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	/** SpringArm controlling camera movement in relation to character movement */
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ability system attributes
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** Ability system component, necessary for using GAS */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character | Abilities", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	/** BaseAttributeSet of the player, implements health and stamina */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character | Attributes", meta = (AllowPrivateAccess = "true"))
	class UBaseAttributeSet* AttributeSet;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Ability attributes
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Character | Abilities", meta = (AllowPrivateAccess = "true"))
	TMap<EAbilitySlots, FGameplayAbilitySpecHandle> AbilitySlots;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Abilities", meta = (AllowPrivateAccess = "true"))
	TMap<EAbilitySlots, TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Abilities", meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> InitializationEffects;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Character | Abilities", meta = (AllowPrivateAcces = "true"))
	bool bIsRocketHovering;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Abilities | Rocket Hover", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> FallingEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Character | Abilities | Rocket Hover", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayEffect> LandingEffect;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Control attributes
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	FTransform DefaultMeshTransform;
	struct FActiveGameplayEffectHandle ActiveFallingEffect;

public:	
	/** 
	* Main Update function called every frame
	* 
	* @param DeltaTime time since last update / frame in seconds
	*/
	virtual void Tick(float DeltaTime) override;

	/**
	* Getter for the ability system component. Necessary for implementation of the IAbilitySystemInterface class
	*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Attribute Accessors
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	UFUNCTION(BlueprintCallable)
	float GetHealth();

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth();

	UFUNCTION(BlueprintCallable)
	float GetEnergy();

	UFUNCTION(BlueprintCallable)
	float GetMaxEnergy();

	UFUNCTION()
	void OnHealthChange(float CurValue, float MaxValue);

	void EnableRagdollMode();
	void DisableRagdollMode();

	/**
		Try to activate an ability by AbilitySlot
	*/
	void TryActivateAbilityBySlot(EAbilitySlots Slot);

	/**
		End an ability by AbilitySlot
	*/
	void CancelAbilityBySlot(EAbilitySlots Slot);

	/**
		Give the character a GameplayAbility and associate it with an ability slot

		@param GrantedAbility - class of the ability granted
		@param Slot - The ability slot the granted ability will bind to
	*/
	void GrantAbility(TSubclassOf<class UGameplayAbility> GrantedAbility, EAbilitySlots Slot);

	/**
		Remove an ability associated with a slot from a character

		@note TODO: Refactor this to work from the weapon!

		@param Slot - The ability slot the revoked ability is associated with
	*/
	void RevokeAbility(EAbilitySlots Slot);

	virtual void Landed(const FHitResult& Hit) override;
	virtual void Falling() override;

	FORCEINLINE bool IsRocketHovering() { return bIsRocketHovering; }

protected:

	/** 
	* Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;
};
