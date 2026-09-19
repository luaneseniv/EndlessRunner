// Copyright Epic Games, Inc. All Rights Reserved.


#include "EndlessRunnerGameModeBase.h"
#include "FloorTile.h"
#include "Kismet/KismetSystemLibrary.h"

void AEndlessRunnerGameModeBase::BeginPlay()
{
	CreateInitialFloorTiles();
}

void AEndlessRunnerGameModeBase::RestartLevel() const
{
	UKismetSystemLibrary::ExecuteConsoleCommand(GetWorld(), "RestartLevel");
}

void AEndlessRunnerGameModeBase::CreateInitialFloorTiles()
{
	// Create first floor tile and get positions of all lands (Left, Center, Right)
	if (const AFloorTile* Tile = AddFloorTile(false))
	{
		Tile->GetLanesLocation(LaneSwitchValues);
	}

	// Create one more tile that doesn't spawn obstacles
	AddFloorTile(false);
	// AddFloorTile(false);
	
	// Create other floor tiles
	for (int i = 0; i < NumInitializeFloorTiles; i++)
	{
		AddFloorTile(true);
	}
}

AFloorTile* AEndlessRunnerGameModeBase::AddFloorTile(const bool bSpawnObstacle)
{
	if (UWorld* World = GetWorld())
	{
		AFloorTile* Tile = World->SpawnActor<AFloorTile>(FloorTileClass, NextSpawnPoint);

		if (Tile)
		{
			if (bSpawnObstacle)
			{
				Tile->SpawnObstacleOnLanes();
			}
			// Assign new spawn point for next tile
			NextSpawnPoint = Tile->GetAttachTransform();
		}
		return Tile;
	}
	return nullptr;
}

void AEndlessRunnerGameModeBase::UpdateCoins()
{
	Coins += 1;
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
	        3,
	        1.0f,
	        FColor::White,
	        FString::Printf(TEXT("Coin Collected: %d"), Coins)
	        );
}




