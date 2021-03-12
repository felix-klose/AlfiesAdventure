// Fill out your copyright notice in the Description page of Project Settings.


#include "FloatingPlatform.h"

#include "Kismet/KismetMathLibrary.h"
#include "Misc/DateTime.h"
#include "TimerManager.h"

// Sets default values
AFloatingPlatform::AFloatingPlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	// Start movement "backwards" so StartMoving can simply invert it when
	// the platform moves for the first time
	MovementDirection = -1;
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	bIsMoving = false;
	// calculate factor B in sin (B x) so we get a period of 4 * MovementTime
	// (i.e. f(x) -> y : [0, MovementTime] -> [0, 1] where f'(x) is always positive)
	SineFactor = (2 * UKismetMathLibrary::GetPI()) / (4 * FMath::Abs(MovementTime));

	StartLoctation = StartPoint->GetActorLocation();
	MovementDirectionAndDistance = EndPoint->GetActorLocation() - StartPoint->GetActorLocation();

	GetWorldTimerManager().SetTimer(PauseTimer, this, &AFloatingPlatform::StartMoving, PauseTime, false);
	SetActorLocation(StartPoint->GetActorLocation());
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsMoving)
	{
		MovementDeltaTime += DeltaTime;

		if (MovementDeltaTime > MovementTime)
		{
			if (MovementDirection == 1)
				SetActorLocation(EndPoint->GetActorLocation());
			else
				SetActorLocation(StartPoint->GetActorLocation());
			bIsMoving = false;

			GetWorldTimerManager().SetTimer(PauseTimer, this, &AFloatingPlatform::StartMoving, PauseTime, false);
		}
		else {
			float Delta = GetRelativeLocationDelta(MovementDeltaTime);

			if (MovementDirection == -1)
				Delta = 1 - Delta;

			FVector NewLocation = StartLoctation + MovementDirectionAndDistance * Delta;
			SetActorLocation(NewLocation);
		}
	}
}

void AFloatingPlatform::StartMoving()
{
	MovementDirection *= -1;
	MovementDeltaTime = 0;
	bIsMoving = true;
}

float AFloatingPlatform::GetRelativeLocationDelta(float Time)
{
	// Inline or move out
	return FMath::Sin(SineFactor * Time);
}