/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#include "PlayerAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "PlayerCharacter.h"

 /** Initialization method for AnimInsatances */
void UPlayerAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

/** Main update function for AnimInstances */
void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	UpdateAnimationProperties();
}

/** Update attributes used to control animatino */
void UPlayerAnimInstance::UpdateAnimationProperties()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}

	if (Pawn)
	{
		FVector Velocity = Pawn->GetVelocity();
		Velocity.Z = 0.0f;
		Speed = Velocity.Size();
		bIsInAir = Pawn->GetMovementComponent()->IsFalling();

		APlayerCharacter* Player = (APlayerCharacter*) Pawn;
		if (Player)
			bIsRocketHovering = Player->IsRocketHovering();
	}
}