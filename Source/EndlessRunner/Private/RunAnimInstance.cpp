// Copyright 2024 Luan Nguyen. All Rights Reserved.


#include "RunAnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"


void URunAnimInstance::NativeInitializeAnimation()
{
	if (Pawn == nullptr)
	{
		Pawn = TryGetPawnOwner();
	}
	
	if (IsValid(Pawn))
	{
		MovementComponent = Pawn->GetMovementComponent();
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
