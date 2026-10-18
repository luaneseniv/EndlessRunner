// Copyright 2024 Luan Nguyen. All Rights Reserved.


#include "UI/RunWidgetController.h"
#include "RunCharacter.h"
#include "EndlessRunner/EndlessRunnerGameModeBase.h"

void URunWidgetController::SetupWidgetController(AGameModeBase* InGameMode, ACharacter* InCharacter)
{
	RunCharacter = CastChecked<ARunCharacter>(InCharacter);
	RunGameMode = CastChecked<AEndlessRunnerGameModeBase>(InGameMode);

	RunGameMode->OnCoinUpdated.BindUObject(this, &ThisClass::UpdateNumCoins);

}

void URunWidgetController::StartRunDistanceTimer(float UpdatingRate)
{
	if (!GetWorld()) return;

	GetWorld()->GetTimerManager().SetTimer(
		DistanceUpdateTimerHandle,
		this, 
		&ThisClass::UpdateRunDistance,
		UpdatingRate,
		true);
}

void URunWidgetController::StopRunDistanceTimer()
{
	if (DistanceUpdateTimerHandle.IsValid() && GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(DistanceUpdateTimerHandle);
		DistanceUpdateTimerHandle.Invalidate();
	}
}

void URunWidgetController::UpdateRunDistance()
{
	if (!RunCharacter->IsAlive()) return;

	int32 RunDistance = RunCharacter->GetRunDistance();
	OnDistanceUpdated.ExecuteIfBound(RunDistance);
}

void URunWidgetController::UpdateNumCoins(int32 NumCoins)
{
	OnCoinUpdated.ExecuteIfBound(NumCoins);
}

void URunWidgetController::BroadcastInitialValues()
{
	StartRunDistanceTimer(DistanceUpdateInterval);
	UpdateNumCoins(RunGameMode->GetNumCoins());
}
