// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "Slide_AN.generated.h"

class IActionInterface;
/**
 * 
 */
UCLASS()
class ENDLESSRUNNER_API USlide_AN : public UAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};
