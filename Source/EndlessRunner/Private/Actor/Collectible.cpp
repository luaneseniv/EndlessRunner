// Copyright 2024 Luan Nguyen. All Rights Reserved.



#include "Actor/Collectible.h"
#include "FloorTile.h"
#include "Component/PoolActorComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/Gameplaystatics.h"

#include "RunCharacter.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot =  CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

}

UPoolActorComponent* ACollectible::GetPoolActorComponent()
{
	return nullptr;
}

void ACollectible::OnCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (PickupSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, this->GetActorLocation());
	}

}

void ACollectible::UpdateAssignedTile(AFloorTile* InNewTile)
{
	UPoolActorComponent* PoolActorComp = this->GetPoolActorComponent();
	if (!PoolActorComp) return;

	// Cleanup previous delegate binding
	if (AssignedTile.IsValid())
	{
		AssignedTile.Get()->OnEndOfTile.RemoveAll(PoolActorComp);
	}

	// Bind to new tile
	if (InNewTile)
	{
		InNewTile->OnEndOfTile.AddUObject(PoolActorComp, &UPoolActorComponent::ScheduleRecycle);
		AssignedTile = InNewTile;
	}
	else
	{
		AssignedTile = nullptr;
	}
	
}

