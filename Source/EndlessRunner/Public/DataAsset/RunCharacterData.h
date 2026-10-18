// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EndlessRunner/EndlessRunner.h"
#include "RunCharacterData.generated.h"

class UInputMappingContext;
class UInputAction;
/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API URunCharacterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category= "Input")
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY(EditAnywhere, Category= "Input", DisplayName= "Imput Action Move")
	TObjectPtr<UInputAction> ActionMove;

	UPROPERTY(EditAnywhere, Category= "Input", DisplayName= "Imput Action Jump")
	TObjectPtr<UInputAction> ActionJump;

	UPROPERTY(EditAnywhere, Category = "Input", DisplayName = "Input Action Slide")
	TObjectPtr<UInputAction> ActionSlide;

	UPROPERTY(EditAnywhere, Category="Animation Montage")
	TObjectPtr<UAnimMontage> SlideMontage;

	// The amount of force that pulls the character down when jumping
	UPROPERTY(EditAnywhere, Category="Action")
	float PullDownAmount = 500.0f;

	UPROPERTY(EditAnyWhere, Category = "Trace Hit")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObstacleTypes;

	UPROPERTY(EditAnyWhere, Category = "Trace Hit")
	bool bDrawDebug;
};
