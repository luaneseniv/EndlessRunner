// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Components/BoxComponent.h"
#include "EndlessRunner/EndlessRunnerGameModeBase.h"
#include "RunCharacter.h"
#include "Obstacle.h"
#include "Actor/Coin.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);
	
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Floor Mesh");
	FloorMesh->SetupAttachment(RootComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>("Attach Point");
	AttachPoint->SetupAttachment(RootComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>("Center Lane");
	CenterLane->SetupAttachment(RootComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>("Left Lane");
	LeftLane->SetupAttachment(RootComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>("Right Lane");
	RightLane->SetupAttachment(RootComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>("Floor Trigger Box");
	FloorTriggerBox->SetupAttachment(RootComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.0f, 500.0f, 200.0f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	// Add callback to trigger box when player overlaps -> Spawn tile
	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlap);

}


// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	// Get GameMode
	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
	check(RunGameMode)
}

void AFloorTile::GetLanesLocation(TArray<float>& LanesLocation) const
{
	LanesLocation.Empty();
	// has to be in this order Left -> Center -> Right
	LanesLocation.Add(LeftLane->GetComponentLocation().Y);
	LanesLocation.Add(CenterLane->GetComponentLocation().Y);
	LanesLocation.Add(RightLane->GetComponentLocation().Y);
}

void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		RunGameMode->AddFloorTile(true);

		GetWorldTimerManager().SetTimer(DestroyHandle, this, &AFloorTile::DestroyFloorTile, 2.0f, false);
	}
}

void AFloorTile::DestroyFloorTile()
{
	if (DestroyHandle.IsValid())
	{
		GetWorldTimerManager().ClearTimer(DestroyHandle);
	}

	this->Destroy();
}


void AFloorTile::SpawnObstacle(UArrowComponent* Lane)
{
	UWorld* World = GetWorld();
	if (World != nullptr && ObstacleClasses.IsEmpty() == false)
	{
		FActorSpawnParameters SpawnParameters;
		// Always spawn Obstacle, avoid collision issues
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		const FTransform SpawningTransform = Lane->GetComponentTransform();

		// depend on spawn rate, decide to spawn obstacle or not
		if (ShouldSpawnObstacle())
		{
			// Get random obstacle class and spawn it
			const uint32 SelectedIndex = FMath::RandRange(0, ObstacleClasses.Num() - 1);
			const TSubclassOf<AObstacle> SelectedObstacleClass = ObstacleClasses[SelectedIndex];

			const FVector TestLocation = FVector::ZeroVector;
			const FRotator TestRotator = FRotator::ZeroRotator;
		
			if (SelectedObstacleClass != nullptr)
			{
				World->SpawnActor<AObstacle>(SelectedObstacleClass, SpawningTransform, SpawnParameters);
			}
		}
		else
		{
			// If the Obstacle is not spawned -> Spawn coin
			SpawnCoin(SpawningTransform);
		}

		// TEST
		SpawnCoinsOnLane(SpawningTransform);
	}
}

void AFloorTile::SpawnObstacleOnLanes()
{
	if (!ObstacleClasses.IsEmpty())
	{
		SpawnObstacle(CenterLane);
		SpawnObstacle(LeftLane);
		SpawnObstacle(RightLane);
	}
}

ACoin* AFloorTile::SpawnCoin(const FTransform& SpawnTransform)
{
	UWorld* World = GetWorld();
	if (CoinClass != nullptr && World != nullptr)
	{
		FActorSpawnParameters SpawnParameters;
		SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		
		return World->SpawnActor<ACoin>(CoinClass, SpawnTransform, SpawnParameters);
	}
	return nullptr;
}


