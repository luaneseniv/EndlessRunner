// Copyright 2024 Luan Nguyen. All Rights Reserved.

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

UCLASS(Abstract)
class ENDLESSRUNNER_API ARunCharacter : public ACharacter, public IActionInterface
{
	GENERATED_BODY()

public:
	// -------------------------
	// FUNCTIONS
	ARunCharacter();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// ~ IActionInterface
	virtual void I_EndSlide_AN() override;
	// ~ End IActionInterface
	
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

	int32 GetRunDistance() const;
	FORCEINLINE bool IsAlive() const { return !bIsDeath; }

	
	// -------------------------
	// PROPERTIES
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 CurrentLane = 1;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite)
	int32 NextLane = 2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void SwitchLand(const FInputActionValue& InputValue);
	void Slide();
	void EndSlide();
	FORCEINLINE bool CanSlide() const { return !bIsSliding; }

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = true));
	TObjectPtr<UCameraComponent> Camera;
	
	UPROPERTY(EditAnywhere, Category="Endless Runner|Character Data")
	TObjectPtr<URunCharacterData> CharacterData;
	
	UPROPERTY(Transient)
	TObjectPtr<AEndlessRunnerGameModeBase> RunGameMode;
	
	UPROPERTY()
	bool bCanSwitchLane = true;

	UPROPERTY()
	bool bIsSliding = false;

	UPROPERTY(Transient)
	FTimerHandle RestartGameHandle;

	UPROPERTY()
	bool bIsDeath = false;
	
};
