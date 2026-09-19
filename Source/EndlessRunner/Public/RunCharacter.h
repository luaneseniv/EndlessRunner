// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ActionInterface.h"
#include "RunCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;
class AEndlessRunnerGameModeBase;
class URunCharacterData;
struct FInputActionValue;

UCLASS()
class ENDLESSRUNNER_API ARunCharacter : public ACharacter, public IActionInterface
{
	GENERATED_BODY()

public:
	// -------------------------
	// FUNCTIONS
	ARunCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintImplementableEvent, Category="Endless Runner|Lane")
	void OnChangeLane();

	UFUNCTION(BlueprintCallable, Category = "Endless Runner|Lane")
	void ChangeLaneUpdate(const float Value);

	UFUNCTION(BlueprintCallable, Category="Endless Runner|Lane")
	void ChangeLaneFinish();

	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void Death();

	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void OnCharacterDeath();

	UFUNCTION(BlueprintCallable, Category="Endless Runner")
	void CollectCoin();
	
	// ~ IActionInterface
	virtual void I_EndSlide_AN() override;
	// ~ End IActionInterface
	
	// -------------------------
	// PROPERTIES
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void SwitchLand(const FInputActionValue& InputValue);
	
	UFUNCTION()
	void Slide();

	UFUNCTION()
	void EndSlide();

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(VisibleInstanceOnly, Category = "References")
	TObjectPtr<AEndlessRunnerGameModeBase> RunGameMode;
	
	UPROPERTY(EditAnywhere, Category="Character Data")
	TObjectPtr<URunCharacterData> CharacterData;

	UPROPERTY()
	bool bIsSliding = false;
	FORCEINLINE bool CanSlide() const { return !bIsSliding; }

	UPROPERTY()
	FTimerHandle RestartGameHandle;

	UPROPERTY()
	bool bIsDeath = false;
	FORCEINLINE bool IsAlive() const { return !bIsDeath; }
};
