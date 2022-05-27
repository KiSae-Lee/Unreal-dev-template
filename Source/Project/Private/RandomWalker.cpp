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
	bCalled = false;
	bShowIndicator = true;
}

// Called when the game starts or when spawned
void ARandomWalker::BeginPlay()
{
	Super::BeginPlay();

	// if CurveFloat is not null.
	if (CurveFloat)
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
		// Update visited location.
		VisitedLocation.Add(ActorInitialLocation);
		TargetLocation += GetRandomDestination();

		// Additional options.
		ShowIndicator();
		// Debug.
		ShowDebugScreenMessage(TargetLocation.ToString(), FColor::Blue);

		//Starting the timeline.
		MovementTimeLine.PlayFromStart();
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
	// TODO(Switch move variable to global)
	FString direction;
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

	// Debug.
	ShowDebugScreenMessage(direction, FColor::Blue);

	return move;
}


void ARandomWalker::ProcessMovementTimeline(float Value)
{
	if (!bCalled)
	{
		ShowDebugScreenMessage("Now Timeline is running!", FColor::Green);
		bCalled = true;
	}

	//Setting up the new location of our actor
	FVector NewLocation = FMath::Lerp(ActorInitialLocation, TargetLocation, Value);
	SetActorLocation(NewLocation);

	if (GetActorLocation() == TargetLocation)
	{
		ShowDebugScreenMessage(GetActorLocation().ToString(), FColor::Cyan);
		bCalled = false;
	}
}

void ARandomWalker::OnEndMovementTimeLine()
{
	ShowDebugScreenMessage("Now Timeline is ended!", FColor::Red);
	ShowDebugScreenMessage("Setting next move...", FColor::Green);
	
	FVector TempTarget;
	TArray<FVector> CheckedLocation;
	
	ActorInitialLocation = TargetLocation;
	TargetLocation += GetRandomDestination();
	
	// TODO(Update visited location and not to visit already visited location.)
	/*
	VisitedLocation.Add(ActorInitialLocation);
	do
	{
		TempTarget = TargetLocation;
		TempTarget += GetRandomDestination();
		if (CheckedLocation.Num() == 0 || !CheckedLocation.Contains(TempTarget))
		{
			CheckedLocation.Add(TempTarget);
			if (CheckedLocation.Num() == 4)
			{
				MovementTimeLine.Stop();
			}
		}
	}
	while (VisitedLocation.Contains(TempTarget));
	TargetLocation = TempTarget;
	*/	

	//Additional options.
	ShowIndicator();

	MovementTimeLine.PlayFromStart();
}

void ARandomWalker::ShowIndicator()
{
	// When Indicator option activated.	
	if (bShowIndicator)
	{
		// Spawn indicator
		GetWorld()->SpawnActor(IndicatorBP)->SetActorLocation(ActorInitialLocation);
		UpdateVisitedLocation();
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
