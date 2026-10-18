// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/ActorPoolInterface.h"
#include "Collectible.generated.h"

class AFloorTile;

UCLASS(Abstract)
class ENDLESSRUNNER_API ACollectible : public AActor, public IActorPoolInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACollectible();
	
	virtual UPoolActorComponent* GetPoolActorComponent() override;

	void UpdateAssignedTile(AFloorTile* InNewTile);


	// ==============
	// PROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Endless Runner|Sound")
	TObjectPtr<USoundBase> PickupSound;

protected:
	UFUNCTION(BlueprintCallable, Category="Endless Runner|Collectible")
	virtual void OnCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
private:
	UPROPERTY()
	TWeakObjectPtr<AFloorTile> AssignedTile;
};
