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
}

// Called when the game starts or when spawned
void AFloatingPlatform::BeginPlay()
{
	Super::BeginPlay();
	
	SinePeriod = UKismetMathLibrary::GetPI() * 2.0f;
	SinePeriod /= AccelerationTime * 4.0f;

	AccelerationDistance = (MaxSpeed - MaxSpeed * UKismetMathLibrary::Cos(SinePeriod * AccelerationTime)) / SinePeriod;

	GetWorldTimerManager().SetTimer(AccelerationTimer, this, &AFloatingPlatform::StartAccelerating, PauseTime, false);

	SetActorLocation(TargetActors[0]->GetActorLocation());
	IndexDirection = 1;

	float MaxTravelDistance = GetDistanceToNextStop();
	AccelerationDistance = FMath::Min(AccelerationDistance, MaxTravelDistance / 2);
}

// Called every frame
void AFloatingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsAccelerating)
	{
		if (CurrentAccelerationTime > AccelerationTime)
		{
			FVector loc = GetActorLocation();
			float CalcAccelerationDistance = (MaxSpeed - MaxSpeed * UKismetMathLibrary::Cos(SinePeriod * AccelerationTime)) / SinePeriod;
			CurrentSpeed = MaxSpeed;
			bIsAccelerating = false;
		}
		else
		{
			CurrentAccelerationTime += DeltaTime;
			CurrentSpeed = MaxSpeed * UKismetMathLibrary::Sin(SinePeriod * CurrentAccelerationTime);
		}
	} else if(bIsBreaking)
	{
		if (CurrentAccelerationTime > AccelerationTime)
		{
			CurrentSpeed = 0;
			bIsBreaking = false;

			FVector NextLocation;
			if (IndexDirection == -1)
				NextLocation = TargetActors[0]->GetActorLocation();
			else
				NextLocation = TargetActors[TargetActors.Num() - 1]->GetActorLocation();
			SetActorLocation(NextLocation);
			GetWorldTimerManager().SetTimer(AccelerationTimer, this, &AFloatingPlatform::StartAccelerating, PauseTime, false);
		}
		else
		{
			CurrentAccelerationTime += DeltaTime;
			CurrentSpeed = MaxSpeed - MaxSpeed * UKismetMathLibrary::Sin(SinePeriod * CurrentAccelerationTime);
		}
	}

	if (CurrentSpeed > 0)
	{
		float DistanceToStop = GetDistanceToNextStop();

		if (GetDistanceToNextStop() < AccelerationDistance && !bIsBreaking)
		{
			StartBreaking();
		}

		FVector NextLocation = TargetActors[CurrentTargetIndex]->GetActorLocation();
		float DistanceToNextTarget = FVector::Distance(GetActorLocation(), NextLocation);

		if (CurrentSpeed * DeltaTime > DistanceToNextTarget)
		{

			if (CurrentTargetIndex == 0 && IndexDirection == -1)
			{
				CurrentSpeed = 0;
				bIsAccelerating = false;
				bIsBreaking = false;
				SetActorLocation(NextLocation);
				GetWorldTimerManager().SetTimer(AccelerationTimer, this, &AFloatingPlatform::StartAccelerating, PauseTime, false);
			}
			else if (CurrentTargetIndex == TargetActors.Num() - 1 && IndexDirection == 1)
			{
				CurrentSpeed = 0;
				bIsAccelerating = false;
				bIsBreaking = false;
				SetActorLocation(NextLocation);
				GetWorldTimerManager().SetTimer(AccelerationTimer, this, &AFloatingPlatform::StartAccelerating, PauseTime, false);
			}
			else
			{
				CurrentTargetIndex += IndexDirection;
				NextLocation = TargetActors[CurrentTargetIndex]->GetActorLocation();
			}
		}

		FVector Direction = NextLocation - GetActorLocation();
		Direction.Normalize();
		AddActorWorldOffset(Direction * CurrentSpeed * DeltaTime);
	}
}

void AFloatingPlatform::StartAccelerating()
{
	bIsBreaking = false;
	bIsAccelerating = true;
	CurrentAccelerationTime = 0;

	if (CurrentTargetIndex == TargetActors.Num() - 1)
	{
		CurrentTargetIndex--;
		IndexDirection = -1;
	}
	else
	{
		CurrentTargetIndex = 1;
		IndexDirection = 1;
	}
}

void AFloatingPlatform::StartBreaking()
{
	bIsAccelerating = false;
	bIsBreaking = true;
	CurrentAccelerationTime = 0;
}

float AFloatingPlatform::GetDistanceToNextStop()
{
	FVector CurLocation = GetActorLocation();
	float result = FVector::Distance(CurLocation, TargetActors[CurrentTargetIndex]->GetActorLocation());

	int i = CurrentTargetIndex + IndexDirection;

	while (i >= 0 && i <= TargetActors.Num() - 1)
	{
		result += FVector::Distance(TargetActors[i - IndexDirection]->GetActorLocation(), TargetActors[i]->GetActorLocation());
		i += IndexDirection;
	}

	return result;
}