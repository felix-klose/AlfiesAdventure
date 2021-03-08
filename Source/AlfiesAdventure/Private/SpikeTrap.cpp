// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikeTrap.h"

#include "Components/StaticMeshComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASpikeTrap::ASpikeTrap()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);

	AbilitySystem = CreateDefaultSubobject<UAbilitySystemComponent>("AbilitySystem");
}

// Called when the game starts or when spawned
void ASpikeTrap::BeginPlay()
{
	Super::BeginPlay();
	OnHitAbilitySpec = AbilitySystem->GiveAbility(FGameplayAbilitySpec(OnHitAbility, 1));
	AbilitySystem->RefreshAbilityActorInfo();
	Mesh->OnComponentHit.AddDynamic(this, &ASpikeTrap::OnHit);
}

// Called every frame
void ASpikeTrap::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

UAbilitySystemComponent* ASpikeTrap::GetAbilitySystemComponent() const
{
	return AbilitySystem;
}

void ASpikeTrap::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{
		HitTarget = OtherActor;
		ACharacter* OtherCharacter = dynamic_cast<ACharacter*>(OtherActor);
		if (OtherCharacter)
		{
			// Get the direction the player hit the collider from
			FVector localPosition = this->GetTransform().GetLocation();
			FVector targetPosition = OtherCharacter->GetTransform().GetLocation();
			FVector direction = targetPosition - localPosition;

			// Lock it onto the XY-Plane
			direction.Z = 0;
			direction.Normalize();
			
			// Rotate it by 30 degrees (Magic Number!) "up", i.e. around the vector perpendicular to it on the XY-Plane
			FVector directionNormal = FVector::CrossProduct(FVector::UpVector, direction);
			direction = direction.RotateAngleAxis(30, directionNormal);

			// Make sure the respective vector goes to positive z
			if (direction.Z < 0)
				direction.Z *= -1;

			// Normalize to gain control over the amount of force applied :)
			direction.Normalize();

			OtherCharacter->LaunchCharacter(direction * 500, true, false);
		}
		AbilitySystem->TryActivateAbility(OnHitAbilitySpec, true);
	}
}