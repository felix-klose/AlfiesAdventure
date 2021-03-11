// Fill out your copyright notice in the Description page of Project Settings.


#include "CollectableCoin.h"
#include "Kismet/GameplayStatics.h"

#include "../AlfiesAdventureGameModeBase.h"

// Sets default values
ACollectableCoin::ACollectableCoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	Mesh->OnComponentBeginOverlap.AddDynamic(this, &ACollectableCoin::OnBeginOverlap);
}

// Called when the game starts or when spawned
void ACollectableCoin::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACollectableCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FRotator rotator = this->GetActorRotation();
	rotator.Yaw += RotationSpeed * DeltaTime;
	this->SetActorRotation(rotator);
}

void ACollectableCoin::OnBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (OtherActor == PlayerPawn)
		{
			AAlfiesAdventureGameModeBase* GameMode = (AAlfiesAdventureGameModeBase*)GetWorld()->GetAuthGameMode();
			GameMode->AddCollectedCoins(Value);
			Destroy();
		}
	}
}