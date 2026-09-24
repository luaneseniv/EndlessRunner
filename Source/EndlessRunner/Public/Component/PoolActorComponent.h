// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "PoolActorComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ENDLESSRUNNER_API UPoolActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPoolActorComponent();

	UFUNCTION()
	bool Acquire(const FTransform& InTransform);

	UFUNCTION()
	void Recycle();

	UFUNCTION()
	void ScheduleRecycle(float InDelay);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void ActivateActor(const FTransform& InTransform);

	UFUNCTION()
	void DeactivateActor();

	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	UPROPERTY()
	bool bShouldOwnerTick;

	UPROPERTY()
	FTimerHandle DeactivateTimerHandle;

	UPROPERTY()
	bool bCanAcquire;

	UPROPERTY()
	bool bIsRecycling;
		
};
