// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "FloorTile.generated.h"

class AObstacle;
class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class AEndlessRunnerGameModeBase;
class ACoin;

/*
 * 
 */
UCLASS()
class ENDLESSRUNNER_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	AFloorTile();

	/* Get location to spawn next floor tile */
	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return AttachPoint->GetComponentTransform();
	}

	/*
	 * Get Y location of lanes on the first floor tile.
	 * This value will be used to determine the location of character when switching lanes.
	 */
	UFUNCTION()
	void GetLanesLocation(TArray<float>& LanesLocation) const;

	/* Spawn Obstacle on a lane */
	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void SpawnObstacle(UArrowComponent* Lane);

	/* Spawn Obstacle on a floor tile */
	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void SpawnObstacleOnLanes();

	/* Spawn a coin with transform */
	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	ACoin* SpawnCoin(const FTransform& SpawnTransform);

	/* Spawn Coins in one Lane */
	UFUNCTION(BlueprintImplementableEvent, Category="Endless Runner")
	void SpawnCoinsOnLane(const FTransform& LaneTransform);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	virtual void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void DestroyFloorTile();

	FORCEINLINE
	bool ShouldSpawnObstacle() const
	{
		return FMath::RandRange(0.0f, 1.0f) <= ObstacleSpawnRate;
	}

	
	// --------------------
	// PROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Endless Runner|Components")
	TObjectPtr<USceneComponent> SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Endless Runner|Components")
	TObjectPtr<UStaticMeshComponent> FloorMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Endless Runner|Components")
	TObjectPtr<UArrowComponent> AttachPoint;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Endless Runner|Components")
	TObjectPtr<UArrowComponent> CenterLane;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Endless Runner|Components")
	TObjectPtr<UArrowComponent> LeftLane;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Endless Runner|Components")
	TObjectPtr<UArrowComponent> RightLane;
	
	/* Used to determine when to spawn a new floor tile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Endless Runner|Components")
	TObjectPtr<UBoxComponent> FloorTriggerBox;
	
	UPROPERTY(VisibleInstanceOnly, Category="Endless Runner|References")
	TObjectPtr<AEndlessRunnerGameModeBase> RunGameMode;
	
	/* Classes of Obstacles to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Endless Runner|Obstacle")
	TArray<TSubclassOf<AObstacle>> ObstacleClasses;
	
	/* Chance to spawn Obstacle on a lane (0-1) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Endless Runner|Obstacle", meta = (ClampMin = 0, ClampMax = 1))
	float ObstacleSpawnRate = 0.5f;

	/* Class of Coin to spawn */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Endless Runner|Collectible")
	TSubclassOf<ACoin> CoinClass;
	
private:
	/* Handle the destroy time of the floor tile */
	UPROPERTY()
	FTimerHandle DestroyHandle;
	
	
};
