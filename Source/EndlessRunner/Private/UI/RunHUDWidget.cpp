// Copyright 2024 Luan Nguyen. All Rights Reserved.


#include "UI/RunHUDWidget.h"
#include "Components/TextBlock.h"
#include "UI/RunWidgetController.h"

void URunHUDWidget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;

	URunWidgetController* RunWidgetController = Cast< URunWidgetController>(WidgetController);
	if (!IsValid(RunWidgetController)) return;

	RunWidgetController->OnCoinUpdated.BindUObject(this, &ThisClass::UpdateCoinText);
	RunWidgetController->OnDistanceUpdated.BindUObject(this, &ThisClass::UpdateDistanceText);
}

void URunHUDWidget::UpdateCoinText(int32 NumCoin)
{
	if (!CoinText) return;

	CoinText->SetText(FText::AsNumber(NumCoin));
}

void URunHUDWidget::UpdateDistanceText(int32 RunDistance)
{
	if (!RunDistanceText) return;

	RunDistanceText->SetText(FText::AsNumber(RunDistance));
}
