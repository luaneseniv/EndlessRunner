// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EndlessRunnerGameModeBase.generated.h"

class UInputAction;
class UInputMappingContext;
class AFloorTile;
class URunWidgetController;
class URunHUDWidget;

DECLARE_DELEGATE_OneParam(FOnCoinUpdated, int32 /*NumCoin*/);

// TODO: Move the actor pool to UActorPoolSubsystem::UWorldSubsystem

USTRUCT(BlueprintType)
struct FPoolArray
{
	GENERATED_BODY()

public:
	FPoolArray() {}

	void Initialize(TSubclassOf<AActor> ActorClass, int PoolSize, UWorld* InWorld);
	AActor* SpawnAvailableActor(const FTransform& InTransform) const;

private:
	UPROPERTY()
	TArray<TObjectPtr<AActor>> Actors;
};

USTRUCT(BlueprintType)
struct FScore
{
	GENERATED_BODY()

public:
	FScore()
		: PlayerName(NAME_None)
		, RunDistance(0)
		, NumCoins(0)
	{ }

	FScore(FName InPlayerName,  int32 InRunDistance, int32 InNumCoins)
		: PlayerName(InPlayerName)
		, RunDistance(InRunDistance)
		, NumCoins(InNumCoins)
	{ }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Endless Runner|Score")
	FName PlayerName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Endless Runner|Score")
	int32 RunDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Endless Runner|Score")
	int32 NumCoins;
};

UCLASS(Abstract)
class ENDLESSRUNNER_API AEndlessRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	void CreateInitialFloorTiles();
	AFloorTile* AddFloorTile();
	void RestartLevel() const;
	void UpdateCoins();

	FScore GetGameScore();
	FORCEINLINE int32 GetNumCoins() const { return CollectedCoins; }

	void SetupPlayerHUD(ACharacter* InCharacter);


	/* Number of initial floor tiles to create */
	UPROPERTY(EditAnyWhere, Category="Endless Runner|Configs")
	int MaxAcquiredFloorTiles = 1;

	/* Time delay in seconds before the actor is returned to the pool */
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
	UPROPERTY(Transient)
	FTransform NextSpawnPoint;

	/* Location to move between lanes */
	UPROPERTY(Transient)
	TArray<float> LaneSwitchValues;

	FOnCoinUpdated OnCoinUpdated;

protected:
	virtual void BeginPlay() override;


	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Endless Runner|UI")
	TSubclassOf<UObject> RunWidgetControllerClass;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Endless Runner|UI")
	TSubclassOf<UUserWidget> HUDWidgetClass;

private:
	void InitializeActorPools();
	void InitializePoolByClass(TSubclassOf<AActor> InClass, int PoolSize);

	UPROPERTY(Transient)
	uint8 CachedSelectedSpawnIndex;

	UPROPERTY(Transient)
	TMap<TSubclassOf<AActor>, FPoolArray> PoolByClasses;

	UPROPERTY(Transient)
	int32 CollectedCoins = 0;

	UPROPERTY(Transient)
	TObjectPtr<URunWidgetController> RunWidgetController;

	UPROPERTY(Transient)
	TObjectPtr<URunHUDWidget> RunHUDWidget;

};
