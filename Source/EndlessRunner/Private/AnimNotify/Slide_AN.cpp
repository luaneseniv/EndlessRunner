// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify/Slide_AN.h"
#include "Interface/ActionInterface.h"

void USlide_AN::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	
	if (const TScriptInterface<IActionInterface> ActionInterface = TScriptInterface<IActionInterface>(MeshComp->GetOwner()))
	{
		ActionInterface->I_EndSlide_AN();
	}
}
