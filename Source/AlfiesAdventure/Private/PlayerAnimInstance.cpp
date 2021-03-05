// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	UpdateAnimationProperties();
}

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
	}
}