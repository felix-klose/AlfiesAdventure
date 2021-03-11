/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AlfiesAdventureGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ALFIESADVENTURE_API AAlfiesAdventureGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	AAlfiesAdventureGameModeBase();

protected:

	int CoinsCollected;

public:

	void AddCollectedCoins(int Value);

	UFUNCTION(BlueprintCallable)
	int GetCoinsCollected();

	UFUNCTION(BlueprintCallable)
	void DisablePlayerInput();

	UFUNCTION(BlueprintCallable)
	void EnablePlayerInput();

	UFUNCTION(BlueprintCallable)
	void RespawnPlayer();
	
};
