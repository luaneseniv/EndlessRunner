// Copyright 2024 Luan Nguyen. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ActorPoolInterface.generated.h"

class UPoolActorComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UActorPoolInterface : public UInterface
{
	GENERATED_BODY()
};


class ENDLESSRUNNER_API IActorPoolInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual UPoolActorComponent* GetPoolActorComponent() = 0;
};
