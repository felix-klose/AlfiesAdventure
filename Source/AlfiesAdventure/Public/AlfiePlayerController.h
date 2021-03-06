/*
 * Copyright(C) Felix Klose - All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 * Written by Felix Klose <felix.klose@felix-klose.tech>, 2021
 */

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AlfiePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class ALFIESADVENTURE_API AAlfiePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// References
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** Character reference. Setup in initialization */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Player Controls | Character", meta = (AllowPrivateAcces = "true"))
	class APlayerCharacter* ControlledCharacter;

public:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Input handlers
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** Move the character forward

		@param Speed - input value used for determining speed and direction
	*/
	UFUNCTION(BlueprintCallable)
	void MoveForward(float Speed);

	/** Move the character right

		@param Speed - input value used for determining speed and direction
	*/
	UFUNCTION(BlueprintCallable)
	void MoveRight(float Speed);

	/** Update Y look direction (up / pitch)

		@param Speed - input value used for determining speed and direction
	*/
	UFUNCTION(BlueprintCallable)
	void LookUp(float Speed);

	/** Update Z look direction (right / yaw)

		@param Speed - input value used for determining speed and direction
	*/
	UFUNCTION(BlueprintCallable)
	void Turn(float Speed);

	/** Perform a jump */
	UFUNCTION(BlueprintCallable)
	void Jump();

	/** Stop jumping */
	UFUNCTION(BlueprintCallable)
	void StopJumping();

protected:

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Utility functions
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	/** Setup the character reference */
	void TryGetCharacterOwner();
};
