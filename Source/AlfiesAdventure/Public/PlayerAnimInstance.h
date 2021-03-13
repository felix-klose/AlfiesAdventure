/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class ALFIESADVENTURE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	// Reference to controlled pawn
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class APawn* Pawn;

	// Current movement speed
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	float Speed;

	// True, iff the controlled pawn is currently not grounded
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Animation Properties", meta = (AllowPrivateAccess = "true"))
	bool bIsRocketHovering;

public:

	/** Initialization method for AnimInsatances */
	virtual void NativeInitializeAnimation() override;
	/** Main update function for AnimInstances */
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	/** Update attributes used to control animatino */
	void UpdateAnimationProperties();
};
