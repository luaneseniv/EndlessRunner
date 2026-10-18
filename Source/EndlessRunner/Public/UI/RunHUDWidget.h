// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RunHUDWidget.generated.h"

class UTextBlock;
class URunWidgetController;

UCLASS()
class ENDLESSRUNNER_API URunHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void SetWidgetController(UObject* InWidgetController);

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Endless Runner|UI", meta=(BindWidget))
	TObjectPtr<UTextBlock> RunDistanceText;

	UPROPERTY(BlueprintReadOnly, Category = "Endless Runner|UI", meta = (BindWidget))
	TObjectPtr<UTextBlock> CoinText;

	UPROPERTY(BlueprintReadOnly, Category="Endless Runner|UI")
	TObjectPtr<UObject> WidgetController;

	UFUNCTION()
	void UpdateCoinText(int32 NumCoin);

	UFUNCTION()
	void UpdateDistanceText(int32 RunDistance);
};
