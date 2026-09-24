// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

// Obstacle collision channel
#define ECC_OBSTACLE ECC_GameTraceChannel1

// Screen Printing
#define SPRINT(Channel, Time, Color, Message, ...) if (GEngine) GEngine->AddOnScreenDebugMessage(Channel, Time, FColor::Color, FString::Printf(TEXT(Message), ##__VA_ARGS__))
#define SPRINT_MESSAGE(Color, Message) SPRINT(-1, 3.0f, Color, Message)
#define SPRINT_VAR(Color, Message,...) SPRINT(-1, 3.0f, Color, Message, ##__VA_ARGS__)

namespace EndlessRunner
{
	/* Collision profile name of the Squeeze Through Obstacle */
	static const FName ObstacleCollisionProfileName(TEXT("SqueezeThroughObstacle"));
	
}
