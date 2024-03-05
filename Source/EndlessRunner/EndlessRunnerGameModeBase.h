// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FloorTile.h"
#include "EndlessRunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, Category="Configs")
	TSubclassOf<AFloorTile> FloorTileClass;

	UPROPERTY(EditAnyWhere, Category="Configs")
	int NumInitializeFloorTiles = 10;

	UPROPERTY(VisibleInstanceOnly, Category="Runtime")
	FTransform NextSpawnPoint;

	UFUNCTION(BlueprintCallable)
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable)
	void AddFloorTile();
	
protected:
	virtual void BeginPlay() override;

};
