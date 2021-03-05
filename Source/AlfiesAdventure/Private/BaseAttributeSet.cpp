// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAttributeSet.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

UBaseAttributeSet::UBaseAttributeSet() : Health(100.f), MaxHealth(100.f), Stamina(100.f), MaxStamina(100.f)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AttributeSet implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Clamp Health and send attribute change event
	if (Data.EvaluatedData.Attribute.GetUProperty() == GetHealthAttribute().GetUProperty())
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));

		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
	// Clamp Stamina and send attribute change event
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetStaminaAttribute().GetUProperty())
	{
		Stamina.SetCurrentValue(FMath::Clamp(Stamina.GetCurrentValue(), 0.f, MaxStamina.GetCurrentValue()));
		Stamina.SetBaseValue(FMath::Clamp(Stamina.GetBaseValue(), 0.f, MaxStamina.GetCurrentValue()));

		OnStaminaChange.Broadcast(Stamina.GetCurrentValue(), MaxStamina.GetCurrentValue());
	}
}