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
	AActor* StartPoint;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floating Platform", meta = (AllowPrivateAcces = "true"))
	AActor* EndPoint;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floating Platform", meta = (AllowPrivateAcces = "true"))
	float MovementTime;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category = "Floating Platform", meta = (AllowPrivateAcces = "true"))
	float PauseTime;

	FTimerHandle PauseTimer;

	bool bIsMoving;
	float MovementDeltaTime;
	float SineFactor;
	int MovementDirection;

	FVector StartLoctation;
	FVector MovementDirectionAndDistance;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartMoving();
	float GetRelativeLocationDelta(float Time);
};
