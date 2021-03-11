/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#include "AlfiePlayerController.h"

#include "GameFramework/Controller.h"

#include "PlayerCharacter.h"

void AAlfiePlayerController::MoveForward(float Speed)
{
	if (ControlledCharacter == nullptr)
	{
		TryGetCharacterOwner();
	}

	if (ControlledCharacter)
	{
		// Current Character Forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
		// Remember: X is forward...
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		if (Speed > 0)
		{
			ControlledCharacter->AddMovementInput(Direction, Speed);
		}
	}
}

void AAlfiePlayerController::MoveRight(float Speed)
{
	if (ControlledCharacter == nullptr)
	{
		TryGetCharacterOwner();
	}

	if (ControlledCharacter)
	{
		// Current Character Forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation = FRotator(0.f, Rotation.Yaw, 0.f);
		// Remember: Y is right...
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		ControlledCharacter->AddMovementInput(Direction, Speed);
	}
}

void AAlfiePlayerController::LookUp(float Speed)
{
	if (ControlledCharacter == nullptr)
	{
		TryGetCharacterOwner();
	}

	if (ControlledCharacter)
	{
		ControlledCharacter->AddControllerPitchInput(Speed);
	}
}

void AAlfiePlayerController::Turn(float Speed)
{
	if (ControlledCharacter == nullptr)
	{
		TryGetCharacterOwner();
	}

	if (ControlledCharacter)
	{
		ControlledCharacter->AddControllerYawInput(Speed);
	}
}

void AAlfiePlayerController::Jump()
{
	if (ControlledCharacter == nullptr)
	{
		TryGetCharacterOwner();
	}

	if (ControlledCharacter)
	{
		ControlledCharacter->Jump();
	}
}

void AAlfiePlayerController::StopJumping()
{
	if (ControlledCharacter == nullptr)
	{
		TryGetCharacterOwner();
	}

	if (ControlledCharacter)
	{
		ControlledCharacter->StopJumping();
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Utility functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void AAlfiePlayerController::TryGetCharacterOwner()
{
	if (ControlledCharacter == nullptr)
	{
		APawn* ControlledPawn = GetPawn();
		if (ControlledPawn != nullptr) {
			// Dynamic casting is safer than just hard-casting. Computational overhead is minimal since
			// under most circumstances this should only be called once per character / instance
			ControlledCharacter = dynamic_cast<APlayerCharacter*>(ControlledPawn);
		}
	}
}