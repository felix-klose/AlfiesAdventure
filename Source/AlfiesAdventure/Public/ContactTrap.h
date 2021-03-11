/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/Actor.h"
#include "GameplayAbilitySpec.h"

#include "ContactTrap.generated.h"

UCLASS()
class ALFIESADVENTURE_API AContactTrap : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AContactTrap();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	/** Ability system component, necessary for useing GAS */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	class UAbilitySystemComponent* AbilitySystem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UGameplayAbility> OnHitAbility;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	FGameplayAbilitySpecHandle OnHitAbilitySpec;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Abilities", meta = (AllowPrivateAccess = "true"))
	AActor* HitTarget;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**
	* Getter for the ability system component. Necessary for implementation of the IAbilitySystemInterface class
	*/
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

};
