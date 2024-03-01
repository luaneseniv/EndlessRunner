// Fill out your copyright notice in the Description page of Project Settings.


#include "RunAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


void URunAnimInstance::NativeInitializeAnimation()
{
	if (Pawn != nullptr)
	{
		MovementComponent = Pawn->GetMovementComponent();

	}
	else
	{
		Pawn = TryGetPawnOwner();
	}

}

void URunAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (Pawn && MovementComponent)
	{
		bIsInAir = MovementComponent->IsFalling();
		
		Speed = Pawn->GetVelocity().Size();
	}
}
