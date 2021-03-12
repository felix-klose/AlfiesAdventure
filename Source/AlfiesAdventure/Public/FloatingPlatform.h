// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloatingPlatform.generated.h"

UCLASS()
class ALFIESADVENTURE_API AFloatingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingPlatform();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Mesh", meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floating Platform", meta = (AllowPrivateAcces = "true"))
	TArray<AActor*> TargetActors;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floating Platform", meta = (AllowPrivateAcces = "true"))
	float MaxSpeed;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floating Platform", meta = (AllowPrivateAcces = "true"))
	float AccelerationTime;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floating Platform", meta = (AllowPrivateAcces = "true"))
	float PauseTime;

	float AccelerationDistance;
	float SinePeriod;
	bool bIsAccelerating = false;
	bool bIsBreaking = false;
	int IndexDirection = 1;

	float CurrentAccelerationTime;
	float CurrentSpeed;
	int CurrentTargetIndex;

	FTimerHandle AccelerationTimer;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartAccelerating();
	void StartBreaking();

	float GetDistanceToNextStop();
};
