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

#include "PlayerCharacter.generated.h"

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

	/** Ability system component, necessary for useing GAS */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character | Abilities", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	/** BaseAttributeSet of the player, implements health and stamina */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Character | Attributes", meta = (AllowPrivateAccess = "true"))
	class UBaseAttributeSet* AttributeSet;

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

protected:

	/** 
	* Called when the game starts or when spawned
	*/
	virtual void BeginPlay() override;
};
