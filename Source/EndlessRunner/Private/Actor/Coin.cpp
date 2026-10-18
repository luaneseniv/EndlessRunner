// Copyright 2024 Luan Nguyen. All Rights Reserved.



#include "Actor/Coin.h"

#include "RunCharacter.h"
#include "Components/SphereComponent.h"
#include "Component/PoolActorComponent.h"
#include "GameFramework/RotatingMovementComponent.h"


ACoin::ACoin()
{
	TriggerSphere = CreateDefaultSubobject<USphereComponent>("Trigger Sphere");
	TriggerSphere->SetupAttachment(RootComponent);

	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>("Coin Mesh");
	CoinMesh->SetupAttachment(RootComponent);
	CoinMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TriggerSphere->OnComponentBeginOverlap.AddDynamic(this, &ACoin::OnCollected);

	PoolActorComponent = CreateDefaultSubobject<UPoolActorComponent>("PoolActorComponent");
}

UPoolActorComponent* ACoin::GetPoolActorComponent()
{
	return PoolActorComponent;
}

void ACoin::OnCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnCollected(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);

	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		RunCharacter->CollectCoin();

		PoolActorComponent->Recycle();
	}
	
}
