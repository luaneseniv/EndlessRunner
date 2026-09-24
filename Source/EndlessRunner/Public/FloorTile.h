// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/ArrowComponent.h"
#include "Interface/ActorPoolInterface.h"
#include "FloorTile.generated.h"

class AObstacle;
class USceneComponent;
class UStaticMeshComponent;
class UBoxComponent;
class AEndlessRunnerGameModeBase;
class ACoin;
class USplineComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnEndOfTile, float /*InDelay*/);

UCLASS(Abstract)
class ENDLESSRUNNER_API AFloorTile : public AActor, public IActorPoolInterface
{
	GENERATED_BODY()

public:
	AFloorTile();

	virtual UPoolActorComponent* GetPoolActorComponent() override;

	/* Get location to spawn next floor tile */
	FORCEINLINE const FTransform& GetAttachTransform() const
	{
		return AttachPoint->GetComponentTransform();
	}

	UFUNCTION()
	void GetCoinTransforms(TArray<FTransform>& OutTransforms) const;

	/*
	* Called only once on the GameMode.
	* Get Y location (breadth) of lanes on the first floor tile.
	* These vectors are used to determine the location of character when switching lanes.
	*/
	UFUNCTION()
	void GetLanesLocation(TArray<float>& LanesLocation) const;

	FOnEndOfTile OnEndOfTile;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void ReleaseFloorTile(float InDelay);

	/* Update the local transform of the coin */
	UFUNCTION()
	void InitializeCoinTransforms();


	// --------------------
	// PROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<UStaticMeshComponent> FloorMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<UArrowComponent> AttachPoint;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<UArrowComponent> CenterLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<UArrowComponent> LeftLane;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<UArrowComponent> RightLane;

	/* Used to determine when to spawn a new floor tile */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<UBoxComponent> FloorTriggerBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Endless Runner|Components")
	TObjectPtr<USplineComponent> CoinSpline;

private:

	// --------------------
	// PROPERTIES
	UPROPERTY()
	TObjectPtr<AEndlessRunnerGameModeBase> RunGameMode;

	/* Handles the destroy time of the floor tile */
	UPROPERTY()
	FTimerHandle DestroyHandle;

	UPROPERTY()
	TArray<FTransform> CoinTransforms;

	UPROPERTY()
	TObjectPtr<UPoolActorComponent> PoolActorComponent;
};