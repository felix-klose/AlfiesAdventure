// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"

#include "BaseAttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)


/** Delegate for handling attribute changes */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAttributeChangeDelegate, float, CurValue, float, MaxValue);
/**
 * 
 */
UCLASS()
class ALFIESADVENTURE_API UBaseAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:

	/** Default constructor, initializing attributes with default values */
	UBaseAttributeSet();

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Base character attributes
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** Current Health of the owning character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttributeSetBase)
		FGameplayAttributeData Health;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Health)

	/** Max Health of the owning character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttributeSetBase)
	FGameplayAttributeData MaxHealth;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxHealth)

	/** Current Stamina of the owning character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttributeSetBase)
	FGameplayAttributeData Stamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, Stamina)

	/** Max Stamina of the owning character */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttributeSetBase)
	FGameplayAttributeData MaxStamina;
	ATTRIBUTE_ACCESSORS(UBaseAttributeSet, MaxStamina)

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// AttributeSet implementation
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** Handle attribute changes made by GameplayEffects

		@param Data: Information about the current attribute changes
	*/
	virtual void PostGameplayEffectExecute(const struct FGameplayEffectModCallbackData& Data) override;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Attribute change delegates
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** Attribute change delegate for Health */
	FOnAttributeChangeDelegate OnHealthChange;

	/** Attribute change delegate for Stamina */
	FOnAttributeChangeDelegate OnStaminaChange;
};
