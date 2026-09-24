// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerGameModeBase.h"
#include "EndlessRunner.h"
#include "FloorTile.h"
#include "Actor/Coin.h"
#include "Component/PoolActorComponent.h"
#include "Interface/ActorPoolInterface.h"
#include "Kismet/KismetSystemLibrary.h"

void FPoolArray::Initialize(TSubclassOf<AActor> ActorClass, int PoolSize, UWorld* InWorld)
{
	if (PoolSize <= 0)
	{
		SPRINT_VAR(
			Red,
			"Invalid pool size number %d. Pool size must be larger than 0 - Class = %s",
			PoolSize, *ActorClass->GetDefaultObjectName().ToString());
		return;
	}

	if (!IsValid(ActorClass) || !IsValid(InWorld)) return;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	FTransform SpawnTransform;
	SpawnTransform.SetLocation(FVector(0.0, 0.0, -9000.0));

	for (int i = 0; i < PoolSize; ++i)
	{
		AActor* SpawnedActor = InWorld->SpawnActor(ActorClass, &SpawnTransform, SpawnParams);
		
		Actors.Add(SpawnedActor);
		
	}
}

AActor* FPoolArray::SpawnAvailableActor(const FTransform& InTransform) const
{
	for (AActor* Actor : Actors)
	{
		if (IActorPoolInterface* ActorPoolInterface = Cast<IActorPoolInterface>(Actor))
		{
			UPoolActorComponent* PoolActorComponent = ActorPoolInterface->GetPoolActorComponent();
			if (PoolActorComponent->Acquire(InTransform))
			{
				return Actor;
			}
		}
	}
	SPRINT_MESSAGE(Red, "Pool does not have any available actor!");
	return nullptr;
}


void AEndlessRunnerGameModeBase::BeginPlay()
{
	CachedSelectedSpawnIndex = FloorTileClasses.Num() - 1;
	InitializeActorPools();
	CreateInitialFloorTiles();
}

void AEndlessRunnerGameModeBase::RestartLevel() const
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "RestartLevel");
}

void AEndlessRunnerGameModeBase::InitializeActorPools()
{
	// floor tiles
	for (const TSubclassOf<AActor>& Class : FloorTileClasses)
	{
		InitializePoolByClass(Class, 5);
	}

	// Coin
	InitializePoolByClass(CoinClass,100);

}

void AEndlessRunnerGameModeBase::InitializePoolByClass(TSubclassOf<AActor> InClass, int PoolSize)
{
	if (!InClass->ImplementsInterface(UActorPoolInterface::StaticClass())) return;

	if (GetWorld())
	{
		FPoolArray NewPool;

		NewPool.Initialize(InClass, PoolSize, GetWorld());

		PoolByClasses.Add(InClass, NewPool);
	}
}

void AEndlessRunnerGameModeBase::CreateInitialFloorTiles()
{
	// Create first floor tile and get positions of all lands (Left, Center, Right)
	if (AFloorTile* Tile = AddFloorTile())
	{
		Tile->GetLanesLocation(LaneSwitchValues);
	}
	
	// Create other floor tiles
	for (int i = 0; i < MaxAcquiredFloorTiles; i++)
	{
		AddFloorTile();
	}
}

AFloorTile* AEndlessRunnerGameModeBase::AddFloorTile()
{
	if (FloorTileClasses.IsEmpty()) return nullptr;

	TArray<int> AvailableIndices;
	int NewIndex = CachedSelectedSpawnIndex;

	for (int i = 0; i < FloorTileClasses.Num(); ++i)
	{
		if (i != CachedSelectedSpawnIndex)
			AvailableIndices.Add(i);
	}

	if (!AvailableIndices.IsEmpty())
	{
		NewIndex = AvailableIndices[FMath::RandRange(0, AvailableIndices.Num() - 1)];
		CachedSelectedSpawnIndex = NewIndex;
	}
	
	TSubclassOf<AActor> SelectedClass = FloorTileClasses[NewIndex];

	const FPoolArray& TilePool = PoolByClasses.FindRef(SelectedClass);

	AFloorTile* Tile = Cast<AFloorTile>(TilePool.SpawnAvailableActor(NextSpawnPoint));
	
	if (Tile)
	{
		// Assign new spawn transform for the next tile
		NextSpawnPoint = Tile->GetAttachTransform();

		// Spawn coins on lane
		if (IsValid(CoinClass) && GetWorld())
		{
			TArray<FTransform> CoinTransforms;
			Tile->GetCoinTransforms(CoinTransforms);
			const FPoolArray& CoinPool = PoolByClasses.FindRef(CoinClass);

			for (const FTransform& SpawningTransform : CoinTransforms)
			{
				ACoin* SpawnedCoin = Cast<ACoin>(CoinPool.SpawnAvailableActor(SpawningTransform));
				SpawnedCoin->UpdateAssignedTile(Tile);
			}
		}
	}
	return Tile;
}

void AEndlessRunnerGameModeBase::UpdateCoins()
{
	ColledtedCoins += 1;
	SPRINT(3, 20.0f, White, "Coin Collected: %d", ColledtedCoins);
}

