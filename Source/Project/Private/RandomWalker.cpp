// Fill out your copyright notice in the Description page of Project Settings.


#include "Randomwalker.h"

// Sets default values
ARandomWalker::ARandomWalker()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Custom codes.
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = RootComp;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);

	GridSize = 100;
	bShowIndicator = true;
	AvoidVisitedLocation = true;
}

// Called when the game starts or when spawned
void ARandomWalker::BeginPlay()
{
	Super::BeginPlay();

	TimeToDelay = 3.0f;

	// if CurveFloat is not null.
	if (CurveFloat && IndicatorBP)
	{
		TimeToDelay = CurveFloat->FloatCurve.GetLastKey().Value;

		// Timeline options.
		FOnTimelineFloat ProcessMovement;
		ProcessMovement.BindUFunction(this, FName("ProcessMovementTimeline"));
		MovementTimeLine.AddInterpFloat(CurveFloat, ProcessMovement);
		MovementTimeLine.SetLooping(false);

		FOnTimelineEvent FinishMovement;
		FinishMovement.BindUFunction(this, FName("OnEndMovementTimeLine"));
		MovementTimeLine.SetTimelineFinishedFunc(FinishMovement);

		// Location options.
		ActorInitialLocation = TargetLocation = GetActorLocation();
		TargetLocation = GetRandomDestination();

		// Additional options.
		ShowIndicator();
		UpdateVisitedLocation();

		// Debug.
		ShowDebugScreenMessage("RandomWalker start to play!", FColor::Red);
		ShowDebugScreenMessage(ActorInitialLocation.ToString(), FColor::Blue);
		ShowDebugScreenMessage("Initial Location: " + ActorInitialLocation.ToString(), FColor::Green);
		ShowDebugScreenMessage("Target Location: " + TargetLocation.ToString(), FColor::Cyan);

		//Starting the timeline.
		MovementTimeLine.PlayFromStart();
	}
	else
	{
		// Debug.
		ShowDebugScreenMessage("Please update CurveFloat and Indicator!", FColor::Red);
	}
}

// Called every frame
void ARandomWalker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Time line setup in tick.
	MovementTimeLine.TickTimeline(DeltaTime);
}

FVector ARandomWalker::GetRandomDestination()
{
	FVector TempTargetLocation;
	FVector move;

	RandomStream.GenerateNewSeed();
	float randNum = RandomStream.FRand();

	if (randNum < 0.25f)
	{
		direction = "Forward";
		move = FVector(GridSize, 0, 0);
	}
	else if (randNum >= 0.25f && randNum < 0.5f)
	{
		direction = "Back";
		move = FVector(-GridSize, 0, 0);
	}
	else if (randNum >= 0.5f && randNum < 0.75f)
	{
		direction = "Right";
		move = FVector(0, GridSize, 0);
	}
	else
	{
		direction = "Left";
		move = FVector(0, -GridSize, 0);
	}

	TempTargetLocation = TargetLocation + move;

	return TempTargetLocation;
}

void ARandomWalker::ShowIndicator()
{
	// When Indicator option activated.	
	if (bShowIndicator)
	{
		// Spawn indicator
		GetWorld()->SpawnActor(IndicatorBP)->SetActorLocation(ActorInitialLocation);
		// UpdateVisitedLocation();
	}
}

void ARandomWalker::UpdateVisitedLocation()
{
	VisitedLocation.Add(ActorInitialLocation);
}

void ARandomWalker::ShowDebugScreenMessage(FString Message, FColor Color)
{
	if (bDebug)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			TimeToDelay,
			Color,
			Message);
	}
}

void ARandomWalker::ProcessMovementTimeline(float Value)
{
	//Setting up the new location of our actor
	FVector NewLocation = FMath::Lerp(ActorInitialLocation, TargetLocation, Value);
	SetActorLocation(NewLocation);
}

void ARandomWalker::OnEndMovementTimeLine()
{
	FVector newLocation = GetRandomDestination();
	
	ShowDebugScreenMessage("Last Location: " + Mesh->GetComponentLocation().ToString(), FColor::Cyan);
	
	ActorInitialLocation = TargetLocation;
	UpdateVisitedLocation();
	
	if(AvoidVisitedLocation)
	{
		while(VisitedLocation.Contains(newLocation))
		{
			newLocation = GetRandomDestination();
		}
	}

	ShowDebugScreenMessage(direction, FColor::Cyan);
	TargetLocation = newLocation;

	//Additional options.
	ShowIndicator();

	MovementTimeLine.PlayFromStart();
}
