// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"
#include "Components/BoxComponent.h"
#include "Components/SplineComponent.h"
#include "EndlessRunner/EndlessRunnerGameModeBase.h"
#include "RunCharacter.h"
#include "Obstacle.h"
#include "Actor/Coin.h"
#include "Component/PoolActorComponent.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AFloorTile::AFloorTile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("Scene Root");
	SetRootComponent(SceneRoot);
	
	FloorMesh = CreateDefaultSubobject<UStaticMeshComponent>("Floor Mesh");
	FloorMesh->SetupAttachment(RootComponent);

	AttachPoint = CreateDefaultSubobject<UArrowComponent>("Attach Point");
	AttachPoint->SetupAttachment(RootComponent);

	CenterLane = CreateDefaultSubobject<UArrowComponent>("Center Lane");
	CenterLane->SetupAttachment(RootComponent);

	LeftLane = CreateDefaultSubobject<UArrowComponent>("Left Lane");
	LeftLane->SetupAttachment(RootComponent);

	RightLane = CreateDefaultSubobject<UArrowComponent>("Right Lane");
	RightLane->SetupAttachment(RootComponent);

	FloorTriggerBox = CreateDefaultSubobject<UBoxComponent>("Floor Trigger Box");
	FloorTriggerBox->SetupAttachment(RootComponent);
	FloorTriggerBox->SetBoxExtent(FVector(32.0f, 500.0f, 200.0f));
	FloorTriggerBox->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));

	// Add callback to trigger box when player overlaps -> Accuires new tile and release old tile
	FloorTriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AFloorTile::OnTriggerBoxOverlap);

	CoinSpline = CreateDefaultSubobject<USplineComponent>("Coin Spline");
	CoinSpline->SetupAttachment(GetRootComponent());

	PoolActorComponent = CreateDefaultSubobject<UPoolActorComponent>("PoolActorComponent");
}

UPoolActorComponent* AFloorTile::GetPoolActorComponent()
{
	return PoolActorComponent;
}

void AFloorTile::BeginPlay()
{
	Super::BeginPlay();

	// Get GameMode
	RunGameMode = Cast<AEndlessRunnerGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	
	if (RunGameMode == nullptr) return;

	InitializeCoinTransforms();
}

void AFloorTile::GetLanesLocation(TArray<float>& LanesLocation) const
{
	// |			|			|			|	^ X Axis
	// |			|			|			|	|
	// |	Left	|	Center	|	Right	|	 -> Y Axis
	LanesLocation.Empty();
	// has to be in this order Left -> Center -> Right
	LanesLocation.Add(LeftLane->GetComponentLocation().Y);
	LanesLocation.Add(CenterLane->GetComponentLocation().Y);
	LanesLocation.Add(RightLane->GetComponentLocation().Y);
}


void AFloorTile::OnTriggerBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (RunGameMode == nullptr) return;

	if (ARunCharacter* RunCharacter = Cast<ARunCharacter>(OtherActor))
	{
		RunGameMode->AddFloorTile();
		
		if (OnEndOfTile.IsBound())
		{
			OnEndOfTile.Broadcast(RunGameMode->DeactivateTime);
			OnEndOfTile.Clear();
		}
		ReleaseFloorTile(RunGameMode->DeactivateTime);
	}
}

void AFloorTile::ReleaseFloorTile(float InDelay)
{
	if (PoolActorComponent)
	{
		PoolActorComponent->ScheduleRecycle(InDelay);
	}
}

void AFloorTile::InitializeCoinTransforms()
{
	if (!IsValid(CoinSpline)) return;

	CoinTransforms.Empty();

	float SplineLength = CoinSpline->GetSplineLength();
	int NumCoins = FMath::TruncToInt(SplineLength / RunGameMode->CoinSpacing);

	CoinTransforms.Reserve(NumCoins);

	for (int i = 0; i < NumCoins; ++i)
	{
		FTransform TransformAtDistance = CoinSpline->GetTransformAtDistanceAlongSpline(RunGameMode->CoinSpacing * i, ESplineCoordinateSpace::Local);
		TransformAtDistance.SetLocation(TransformAtDistance.GetLocation() + CoinSpline->GetRelativeLocation() + RunGameMode->CoinOffset);
		TransformAtDistance.SetRotation(FQuat::Identity);

		CoinTransforms.Add(TransformAtDistance);
	}
}

void AFloorTile::GetCoinTransforms(TArray<FTransform>& OutTransforms) const
{
	OutTransforms.Reserve(CoinTransforms.Num());

	for (const FTransform& LocalTransform : CoinTransforms)
	{
		OutTransforms.Emplace(LocalTransform * GetActorTransform());
	}
}

