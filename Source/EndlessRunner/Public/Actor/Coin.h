// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actor/Collectible.h"
#include "Coin.generated.h"

class URotatingMovementComponent;
class USphereComponent;
/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API ACoin : public ACollectible
{
	GENERATED_BODY()

public:
	ACoin();

	// ===================
	// PROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<USphereComponent> TriggerSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<UStaticMeshComponent> CoinMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Component")
	TObjectPtr<URotatingMovementComponent> RotateComponent;

protected:
	virtual void OnCollected(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
