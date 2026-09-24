// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameModeBase.generated.h"

class UInputAction;
class UInputMappingContext;
class AFloorTile;


USTRUCT(BlueprintType)
struct FPoolArray
{
	GENERATED_BODY()

public:
	void Initialize(TSubclassOf<AActor> ActorClass, int PoolSize, UWorld* InWorld);
	AActor* SpawnAvailableActor(const FTransform& InTransform) const;

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Actors;
};

UCLASS(Abstract)
class ENDLESSRUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	// FUNCTIONS
	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void CreateInitialFloorTiles();

	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	AFloorTile* AddFloorTile();

	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void UpdateCoins();
	
	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void RestartLevel() const;

	UFUNCTION()
	void InitializeActorPools();

	UFUNCTION()
	void InitializePoolByClass(TSubclassOf<AActor> InClass, int PoolSize);

	// --------------------
	// PROPERTIES
	/* Number of initial floor tiles to create */
	UPROPERTY(EditAnyWhere, Category="Endless Runner|Configs")
	int MaxAcquiredFloorTiles = 1;

	/* After this time, the object is return to the pool */
	UPROPERTY(EditAnyWhere, Category = "Endless Runner|Configs")
	float DeactivateTime = 1.0f;

	/* Classes of the floor to spawn */
	UPROPERTY(EditAnywhere, Category="Endless Runner|Configs")
	TArray<TSubclassOf<AActor>> FloorTileClasses;

	/* Class of the Coin to spawn */
	UPROPERTY(EditAnywhere, Category = "Endless Runner|Configs")
	TSubclassOf<AActor> CoinClass;

	UPROPERTY(EditAnywhere, Category = "Endless Runner|Configs")
	float CoinSpacing = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Endless Runner|Configs")
	FVector CoinOffset;

	/* Location to spawn the next tile */
	UPROPERTY(BlueprintReadOnly, Category="Endless Runner|Runtime")
	FTransform NextSpawnPoint;

	/* Location to move between lanes */
	UPROPERTY(BlueprintReadOnly, Category="Endless Runner|Floor Tile")
	TArray<float> LaneSwitchValues;

	/* Number of coins collected */
	UPROPERTY(BlueprintReadOnly, Category="Endless Runner|Runtime")
	int32 ColledtedCoins = 0;
	


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	int CachedSelectedSpawnIndex;

	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, FPoolArray> PoolByClasses;
};
