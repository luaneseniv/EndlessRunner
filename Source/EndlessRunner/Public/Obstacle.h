// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Obstacle.generated.h"

UCLASS()
class ENDLESSRUNNER_API AObstacle : public AActor
{
	GENERATED_BODY()
	
public:	
	AObstacle();
	
	UFUNCTION(BlueprintCallable, Category="Obstacle")
	virtual void OnHitObstacle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit );

	// -------------------------
	// PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Obstacle")
	TObjectPtr<UStaticMeshComponent> ObstacleMesh;

	
};
