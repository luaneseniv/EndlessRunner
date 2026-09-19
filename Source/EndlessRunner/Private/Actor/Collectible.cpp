// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/Collectible.h"

#include "RunCharacter.h"

// Sets default values
ACollectible::ACollectible()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot =  CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);

}

void ACollectible::OnCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

}




