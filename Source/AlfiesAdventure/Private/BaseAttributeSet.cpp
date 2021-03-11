/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#include "BaseAttributeSet.h"

#include "GameplayEffect.h"
#include "GameplayEffectExtension.h"

 /** Default constructor, initializing attributes with default values */
UBaseAttributeSet::UBaseAttributeSet() : Health(100.f), MaxHealth(100.f), Energy(100.f), MaxEnergy(100.f)
{
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// AttributeSet implementation
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** Handle attribute changes made by GameplayEffects
*
*	@param Data: Information about the current attribute changes
*/
void UBaseAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	// Clamp Health and send attribute change event
	if (Data.EvaluatedData.Attribute.GetUProperty() == GetHealthAttribute().GetUProperty())
	{
		Health.SetCurrentValue(FMath::Clamp(Health.GetCurrentValue(), 0.f, MaxHealth.GetCurrentValue()));
		Health.SetBaseValue(FMath::Clamp(Health.GetBaseValue(), 0.f, MaxHealth.GetCurrentValue()));

		OnHealthChange.Broadcast(Health.GetCurrentValue(), MaxHealth.GetCurrentValue());
	}
	// Clamp Energy and send attribute change event
	else if (Data.EvaluatedData.Attribute.GetUProperty() == GetEnergyAttribute().GetUProperty())
	{
		Energy.SetCurrentValue(FMath::Clamp(Energy.GetCurrentValue(), 0.f, MaxEnergy.GetCurrentValue()));
		Energy.SetBaseValue(FMath::Clamp(Energy.GetBaseValue(), 0.f, MaxEnergy.GetCurrentValue()));

		OnEnergyChange.Broadcast(Energy.GetCurrentValue(), MaxEnergy.GetCurrentValue());
	}
}