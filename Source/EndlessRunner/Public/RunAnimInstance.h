// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "RunAnimInstance.generated.h"

/**
 * 
 */

class APawn;
class UPawnMovementComponent;

UCLASS()
class ENDLESSRUNNER_API URunAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	float Speed;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	bool bIsInAir;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<APawn> Pawn;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Movement")
	TObjectPtr<UPawnMovementComponent> MovementComponent;

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;


};
