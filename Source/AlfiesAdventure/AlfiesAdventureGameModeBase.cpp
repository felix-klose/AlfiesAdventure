/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#include "AlfiesAdventureGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/Character.h"

#include "Public/PlayerCharacter.h"

void AAlfiesAdventureGameModeBase::DisablePlayerInput()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->DisableInput(PlayerController);
		PlayerController->SetInputMode(FInputModeUIOnly());

		PlayerController->bShowMouseCursor = true;
		PlayerController->bEnableClickEvents = true;
		PlayerController->bEnableMouseOverEvents = true;
	}
}

void AAlfiesAdventureGameModeBase::EnablePlayerInput()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->EnableInput(PlayerController);
		PlayerController->SetInputMode(FInputModeGameOnly());

		PlayerController->bShowMouseCursor = false;
		PlayerController->bEnableClickEvents = false;
		PlayerController->bEnableMouseOverEvents = false;
	}
}

void AAlfiesAdventureGameModeBase::RespawnPlayer()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	AController* PlayerController = (APlayerController*)PlayerPawn->GetController();

	PlayerController->UnPossess();
	PlayerPawn->Destroy();

	FTransform SpawnTransform = FindPlayerStart(PlayerController)->GetTransform();
	APawn* NewPlayer = GetWorld()->SpawnActor<APawn>(this->DefaultPawnClass, SpawnTransform, FActorSpawnParameters());
	PlayerController->Possess(NewPlayer);

	EnablePlayerInput();
} 