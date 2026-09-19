// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
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
};
