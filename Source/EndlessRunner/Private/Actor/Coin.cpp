// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Coin.h"

#include "RunCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/RotatingMovementComponent.h"


ACoin::ACoin()
{
	TriggerSphere = CreateDefaultSubobject<USphereComponent>("Trigger Sphere");
	TriggerSphere->SetupAttachment(RootComponent);

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>("Coin Mesh");
	CoinMesh->SetupAttachment(RootComponent);
	CoinMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	RotateComponent = CreateDefaultSubobject<URotatingMovementComponent>("Rotate Component");
	RotateComponent->SetAutoActivate(true);
	RotateComponent->RotationRate = FRotator(0.0f, 180.0f, 0.0f);

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnCollected);
}

void ACoin::OnCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		RunCharacter->CollectCoin();
		this->Destroy();
	}
	
}
