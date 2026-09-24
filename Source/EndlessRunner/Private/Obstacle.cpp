// Fill out your copyright notice in the Description page of Project Settings.


#include "Obstacle.h"
#include "RunCharacter.h"
#include "Component/PoolActorComponent.h"


// Sets default values
AObstacle::AObstacle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);
	SceneRoot->SetMobility(EComponentMobility::Static);
	
	ObstacleMesh = CreateDefaultSubobject<UStaticMeshComponent>("Obstacle Mesh");
	ObstacleMesh->SetupAttachment(RootComponent);
	ObstacleMesh->SetMobility(EComponentMobility::Static);
	ObstacleMesh->SetCollisionProfileName("BlockAll");

	ObstacleMesh->OnComponentHit.AddDynamic(this, &AObstacle::OnHitObstacle);

}

void AObstacle::OnHitObstacle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	FVector NormalImpulse, const FHitResult& Hit)
{
	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		RunCharacter->Death();
	}
}






