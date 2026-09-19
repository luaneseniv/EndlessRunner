// Fill out your copyright notice in the Description page of Project Settings.


#include "RunCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EndlessRunner/EndlessRunnerGameModeBase.h"
#include "Engine/Engine.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"
#include "DataAsset/RunCharacterData.h"
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
		MovementComp->JumpZVelocity = 510.0f;
		MovementComp->MaxWalkSpeed = 1000.0f;
		MovementComp->AirControl = 0.2f;
	}
}

// Called when the game starts or when spawned
void ARunCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

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
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(
				12,
				1.1f,
				FColor::Green,
				FString::Printf(TEXT("Distance: %fm"), (GetActorLocation().X * 0.01))
				);
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
	FVector Location = GetActorLocation();
	Location.Y = FMath::Lerp(RunGameMode->LaneSwitchValues[CurrentLane], RunGameMode->LaneSwitchValues[NextLane], Value);
	SetActorLocation(Location);
}

// called in blueprint
void ARunCharacter::ChangeLaneFinish()
{
	CurrentLane = NextLane;
}

void ARunCharacter::SwitchLand(const FInputActionValue& InputValue)
{
	const int Value = InputValue.Get<float>();
	
	// clamp to fit the size of LaneSwitchValues array in the GameMode [0, 1, 2] -> 3 lanes
	NextLane = FMath::Clamp(CurrentLane + Value, 0, 2);

	// Implemented in blueprint
	OnChangeLane();
}

void ARunCharacter::Slide()
{
	UCharacterMovementComponent* MovementComponent = GetCharacterMovement();
	const bool bOnGround = MovementComponent->IsFalling() == false;

	if (MovementComponent != nullptr && CanSlide() && bOnGround)
	{
		// Instant push the character down
		const FVector Impulse = FVector(0.0f, 0.0f, -1000.0f);
		MovementComponent->AddImpulse(Impulse, true);

		// Set sliding flag and disable input
		bIsSliding = true;
		DisableInput(nullptr);
		
		// Update Collision Response to Ignore Squeeze through Obstacle
		GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_OBSTACLE, ECR_Ignore);

		// Play Animation montage
		if (CharacterData != nullptr)
		{
			PlayAnimMontage(CharacterData->SlideMontage);
		}
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
