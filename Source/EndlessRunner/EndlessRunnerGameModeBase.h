// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FloorTile.h"
#include "EndlessRunnerGameModeBase.generated.h"

class UInputAction;
class UInputMappingContext;
class AFloorTile;

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// FUNCTIONS
	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	AFloorTile* AddFloorTile(const bool bSpawnObstacle);

	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void UpdateCoins();
	
	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void RestartLevel() const;

	// --------------------
	// PROPERTIES
	/* Class of the floor to spawn */
	UPROPERTY(EditAnywhere, Category="Endless Runner|Floor Tile")
	TSubclassOf<AFloorTile> FloorTileClass;

	/* Number of initial floor tiles to create */
	UPROPERTY(EditAnyWhere, Category="Endless Runner|Floor Tile")
	int NumInitializeFloorTiles = 10;

	/* Location to spawn the next tile */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Endless Runner|Runtime")
	FTransform NextSpawnPoint;

	/* Location to move between lanes */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Endless Runner|Floor Tile")
	TArray<float> LaneSwitchValues;

	/* Number of coins collected */
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category="Endless Runner|Runtime")
	int32 Coins = 0;
	


protected:
	virtual void BeginPlay() override;

	
};
