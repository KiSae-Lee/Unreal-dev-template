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
}

// Called when the game starts or when spawned
void ARandomWalker::BeginPlay()
{
	Super::BeginPlay();

	// if CurveFloat is not null.
	if(CurveFloat)
	{
		TimeToDelay = CurveFloat->FloatCurve.GetLastKey().Value;
		/*The ProgressFunction contains the signature of the function that is
		going to execute every time we tick our timeline.
		Think of the ProgressFunction as a delegate!*/
		FOnTimelineFloat ProcessMovement;
		/*In order to bind the function our UFUNCTION we need to create a FName which contains
		the name of the function we want to call every time the timeline advances.
		Binding the HandleProgress function to the ProgressFunction...*/
		ProcessMovement.BindUFunction(this, FName("ProcessMovementTimeline"));
		//Setting up the loop status and the function that is going to fire when the timeline ticks.
		MovementTimeLine.AddInterpFloat(CurveFloat, ProcessMovement);
		MovementTimeLine.SetLooping(false);

		/*Initializing our home and target location in order to make the actor float between them.
		As you can see the two locations have the same X and Y but their Z value is different.*/
		ActorInitialLocation = TargetLocation = GetActorLocation();
		TargetLocation += GetRandomDestination();

		// Debug message.
		GEngine->AddOnScreenDebugMessage(
		-1,
		2.0f,
		FColor::Blue,
		TargetLocation.ToString());
		
		FOnTimelineEvent FinishMovement;
		FinishMovement.BindUFunction(this,FName("OnEndMovementTimeLine"));
		MovementTimeLine.SetTimelineFinishedFunc(FinishMovement);
		
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
	// debugging tests.
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
	
	// Debug message.
	GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDelay,
		FColor::Blue,
		direction);

	return move;
}


void ARandomWalker::ProcessMovementTimeline(float Value)
{
	if(!bCalled)
	{
		// Debug message.
		GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDelay,
		FColor::Green,
		TEXT("Now Timeline is running!"));
		bCalled = true;
	}
	
	//Setting up the new location of our actor
	FVector NewLocation = FMath::Lerp(ActorInitialLocation, TargetLocation, Value);
	SetActorLocation(NewLocation);
	
	if(GetActorLocation() == TargetLocation)
	{
		// Debug message.
		GEngine->AddOnScreenDebugMessage(
		-1,
		TimeToDelay,
		FColor::Cyan,
		GetActorLocation().ToString());
		bCalled = false;
	}
}

void ARandomWalker::OnEndMovementTimeLine()
{
	// Debug message.
	GEngine->AddOnScreenDebugMessage(
	-1,
	TimeToDelay,
	FColor::Red,
	TEXT("Now Timeline is ended!"));
	
	GEngine->AddOnScreenDebugMessage(
	-1,
	TimeToDelay,
	FColor::Green,
	TEXT("Setting next move..."));
	
	ActorInitialLocation = TargetLocation;
	TargetLocation += GetRandomDestination();
	MovementTimeLine.PlayFromStart();
}

