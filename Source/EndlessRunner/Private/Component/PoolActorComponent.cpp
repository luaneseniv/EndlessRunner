// Copyright 2024 Luan Nguyen. All Rights Reserved.


#include "Component/PoolActorComponent.h"
#include "EndlessRunner/EndlessRunner.h"

UPoolActorComponent::UPoolActorComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPoolActorComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerActor = GetOwner();
	bShouldOwnerTick = OwnerActor->IsActorTickEnabled();
	DeactivateActor();
}

/*
	* Activate the actor and return True if the actor can be acquired.
	* Else return False and do nothing.
*/
bool UPoolActorComponent::Acquire(const FTransform& InTransform)
{
	if (bCanAcquire == false)
		return false;
	else
	{
		ActivateActor(InTransform);
		return true;
	}
}

// Immediately pushes the actor to the pool.
void UPoolActorComponent::Recycle()
{
	if (bCanAcquire == true || bIsRecycling == true) return;

	DeactivateActor();
}

// Schedule to push the actor to the pool after an amount of seconds.
void UPoolActorComponent::ScheduleRecycle(float InDelay)
{
	if (bIsRecycling == true || bCanAcquire == true) return;

	bIsRecycling = true;

	if (!GetWorld())
	{
		SPRINT_MESSAGE(Red, "ScheduleRecycle: GetWorld() is not valid! Can not set timer!");
		DeactivateActor();
		return;
	}

	GetWorld()->GetTimerManager().SetTimer(
		DeactivateTimerHandle,
		this,
		&UPoolActorComponent::DeactivateActor,
		InDelay,
		false);
}

void UPoolActorComponent::ActivateActor(const FTransform& InTransform)
{
	bCanAcquire = false;
	OwnerActor->SetActorTickEnabled(bShouldOwnerTick);
	OwnerActor->SetActorEnableCollision(true);
	OwnerActor->GetRootComponent()->SetVisibility(true, true);
	OwnerActor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	OwnerActor->SetActorTransform(InTransform);
}

void UPoolActorComponent::DeactivateActor()
{
	if (GetWorld() && DeactivateTimerHandle.IsValid())
	{
		GetWorld()->GetTimerManager().ClearTimer(DeactivateTimerHandle);
		DeactivateTimerHandle.Invalidate();
		bIsRecycling = false;
	}

	OwnerActor->SetActorEnableCollision(false);
	OwnerActor->GetRootComponent()->SetVisibility(false, true);
	OwnerActor->GetRootComponent()->SetMobility(EComponentMobility::Static);
	OwnerActor->SetActorTickEnabled(false);
	bCanAcquire = true;
}



