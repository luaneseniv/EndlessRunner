// Copyright 2024 Luan Nguyen. All Rights Reserved.


#include "RunCharacter.h"
//#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EndlessRunner/EndlessRunnerGameModeBase.h"
#include "Engine/Engine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAsset/RunCharacterData.h"
#include "Kismet/KismetSystemLibrary.h"
#include "EndlessRunner/EndlessRunner.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARunCharacter::ARunCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Camera Arm"));
	CameraArm->TargetArmLength = 450.0f;
	CameraArm->bUsePawnControlRotation = false;
	CameraArm->SetRelativeRotation(FRotator(0.0f, -15.0f, 0.0f));
	CameraArm->SetupAttachment(GetRootComponent());

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm);
	Camera->bUsePawnControlRotation = false;

	// Block Squeeze Obstacle by default
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OBSTACLE, ECR_Block);
	
	// Set Default value for Character Movement Component
	if (UCharacterMovementComponent* MovementComp = GetCharacterMovement())
	{
		MovementComp->JumpZVelocity = 600.0f;
		MovementComp->MaxWalkSpeed = 1000.0f;
		MovementComp->AirControl = 0.2f;
	}
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (!IsValid(GetWorld())) return;

	RunGameMode = Cast<AEndlessRunnerGameModeBase>(GetWorld()->GetAuthGameMode());

	if (RunGameMode == nullptr)
	{
		SetActorTickEnabled(false);
		return;
	}

	checkf(CharacterData, TEXT("Character Data is not valid, please check BP_RunCharacter"));

	// Assign Mapping context
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController == nullptr) return;
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	
	Subsystem->AddMappingContext(CharacterData->InputMappingContext, 0);
	
	// Setup Widget controller
	
	RunGameMode->SetupPlayerHUD(this);
}

void ARunCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// infinity run
	FRotator ControlRotation = GetControlRotation();
	ControlRotation.Pitch = 0.0f;
	ControlRotation.Roll = 0.0f;
	AddMovementInput(ControlRotation.Vector());

	if (IsAlive())
	{
		SPRINT(2, 1.0f, Green, "Distance: %fm", (GetActorLocation().X * 0.01));

	}
	
}

// Called to bind functionality to input
void ARunCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Bind Actions
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	EnhancedInputComponent->BindAction(CharacterData->ActionJump, ETriggerEvent::Started, this, &ARunCharacter::Jump);
	EnhancedInputComponent->BindAction(CharacterData->ActionMove, ETriggerEvent::Started, this, &ARunCharacter::SwitchLand);
	EnhancedInputComponent->BindAction(CharacterData->ActionSlide, ETriggerEvent::Started, this, &ARunCharacter::Slide);
	
}

// called in blueprint
void ARunCharacter::ChangeLaneUpdate(const float Value)
{
	if (!RunGameMode)
	{
		SPRINT_CMSG(Red, "ChangeLaneUpdate: Game Mode is not valid!");
		return;
	}

	if (!RunGameMode->LaneSwitchValues.IsEmpty())
	{
		FVector Location = GetActorLocation();
		if (CurrentLane == NextLane) return;

		Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
		SetActorLocation(Location);
	}
}

// called in blueprint
void ARunCharacter::ChangeLaneFinish()
{
	CurrentLane = NextLane;
	bCanSwitchLane = true;
}

void ARunCharacter::SwitchLand(const FInputActionValue& InputValue)
{
	// Block the input while OnChangeLane() is on going
	if (!bCanSwitchLane) return;

	const int Value = InputValue.Get<float>();
	/*
	// Line trace through input axis (Left-right)
	const FVector StartPosition = GetActorLocation();
	const FVector EndPosition = StartPosition + FVector(0.0, Value * 1000.0, 0.0);
	FHitResult HitResult;
	TArray<AActor*> IgnoredActors;

	bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		StartPosition,
		EndPosition,
		CharacterData->ObstacleTypes,
		false,
		IgnoredActors,
		CharacterData->bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
		HitResult,
		true
	);

	// Can not change lane if hit an obstacle
	if (bHit) return;
	*/
	
	// clamp to fit the size of LaneSwitchValues array in the GameMode [0, 1, 2] -> 3 lanes
	NextLane = FMath::Clamp(CurrentLane + Value, 0, 2);

	bCanSwitchLane = false;

	// Implemented in blueprint
	OnChangeLane();
}

void ARunCharacter::Slide()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	const bool bOnGround = MovementComponent->IsFalling() == false;

	if (MovementComponent == nullptr) return;

	if (bOnGround)
	{
		// Return if the char is sliding
		if (!CanSlide()) return;

		// Set sliding flag and disable input
		bIsSliding = true;
		DisableInput(nullptr);
		
		// Line trace through the bottom of the character.
		const FVector StartPosition = GetActorLocation();
		const FVector EndPosition = StartPosition + (GetActorUpVector() * (-1000.0));
		FHitResult HitResult;
		TArray<AActor*> IgnoredActors;

		bool bHit = UKismetSystemLibrary::LineTraceSingleForObjects(
			this,
			StartPosition,
			EndPosition,
			CharacterData->ObstacleTypes,
			false,
			IgnoredActors,
			CharacterData->bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None,
			HitResult,
			true
		);
		
		if (!bHit)
		{
			SPRINT_MSG("Not hitted");
			// If character is NOT slide on top of an obstacle
		// Update Collision Response to Ignore Squeeze through Obstacle
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OBSTACLE, ECR_Ignore);
		}

		// Play slide Animation montage
		if (CharacterData->SlideMontage != nullptr)
		{
			PlayAnimMontage(CharacterData->SlideMontage);
		}
	}
	else
	{
		// Instant pull the character down if it's jumping
		const FVector Impulse = FVector(0.0f, 0.0f, -CharacterData->PullDownAmount);
		MovementComponent->AddImpulse(Impulse, true);
	}
}

void ARunCharacter::EndSlide()
{
	bIsSliding = false;
	EnableInput(nullptr);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OBSTACLE, ECR_Block);
	
}

// Interface functions
// Called when SlideAnimMontage is finished
void ARunCharacter::I_EndSlide_AN()
{
	EndSlide();
}
// ~ End Interface functions

void ARunCharacter::Death()
{
	// called only once
	if (IsAlive())
	{
		bIsDeath = true;
		DisableInput(nullptr);
	
		// Stop Movement
		if (UCharacterMovementComponent* MovementComponent = GetCharacterMovement())
		{
			MovementComponent->StopMovementImmediately();
			MovementComponent->DisableMovement();
		}

		// Ragdoll
		if (USkeletalMeshComponent* SkeletalMeshComponent = GetMesh())
		{
			SkeletalMeshComponent->SetSimulatePhysics(true);
		}

		UnPossessed();

		// restart game after 3 seconds
		GetWorldTimerManager().SetTimer(RestartGameHandle, this, &ARunCharacter::OnCharacterDeath, 3.0f, false);
	}
}

void ARunCharacter::OnCharacterDeath()
{
	bIsDeath = false;
	if (RestartGameHandle.IsValid())
	{
		// Cancel timer
		GetWorldTimerManager().ClearTimer(RestartGameHandle);
	}
	RunGameMode->RestartLevel();

}

void ARunCharacter::CollectCoin()
{
	RunGameMode->UpdateCoins();
}

int32 ARunCharacter::GetRunDistance() const
{
	return FMath::TruncToInt32(GetActorLocation().X * 0.01);
}
