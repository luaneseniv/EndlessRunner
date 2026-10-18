// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RunWidgetController.generated.h"

class AEndlessRunnerGameModeBase;
class ARunCharacter;

DECLARE_DELEGATE_OneParam(FOnIntDataUpdated, int32 /*NewInt*/);

UCLASS(Blueprintable)
class ENDLESSRUNNER_API URunWidgetController : public UObject
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Endless Runner|Configs")
	float DistanceUpdateInterval = 0.06f;

	void SetupWidgetController(AGameModeBase* InGameMode, ACharacter* InCharacter);
	void StartRunDistanceTimer(float UpdatingRate);
	void StopRunDistanceTimer();
	void UpdateRunDistance();
	void UpdateNumCoins(int32 NumCoins);
	void BroadcastInitialValues();

	FOnIntDataUpdated OnCoinUpdated;
	FOnIntDataUpdated OnDistanceUpdated;

private:
	UPROPERTY()
	TObjectPtr<AEndlessRunnerGameModeBase> RunGameMode;

	UPROPERTY()
	TObjectPtr<ARunCharacter> RunCharacter;

	UPROPERTY()
	FTimerHandle DistanceUpdateTimerHandle;
};
