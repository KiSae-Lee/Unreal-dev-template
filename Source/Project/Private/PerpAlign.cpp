// Fill out your copyright notice in the Description page of Project Settings.


#include "PerpAlign.h"

// Sets default values
APerpAlign::APerpAlign()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsDebug = true;
	Count = 5;
}

// Called when the game starts or when spawned
void APerpAlign::BeginPlay()
{
	Super::BeginPlay();

	if(!CurveFloat)
	{
		DebugMessage("Please add CurveFloat!", FColor::Red, 5.0f);
	}
	else if(!Indicator)
	{
		DebugMessage("Please add Indicator!", FColor::Red, 5.0f);
	}
	else
	{
		/*TIMELINE*/
		// Setup debug delay.
		DebugDelay = CurveFloat->FloatCurve.GetLastKey().Value;
		// Setup timeline function.
		FOnTimelineFloat Movement;
		Movement.BindUFunction(this, FName("InProgress"));
		MovementTimeLine.AddInterpFloat(CurveFloat, Movement);
		// Setup end function.
		FOnTimelineEvent Finished;
		Finished.BindUFunction(this, FName("OnFinished"));
		MovementTimeLine.SetTimelineFinishedFunc(Finished);
		// looping setting.
		MovementTimeLine.SetLooping(false);

		/*Test*/
		for (int i = 0; i < Count; ++i)
		{
			AActor* SpawnedActor;
			Vertices.Add(GetRandomInitialLocation());
			SpawnedActor = GetWorld()->SpawnActor(Indicator);
			SpawnedActor->SetActorLocation(Vertices[i]);
			indicators.Add(SpawnedActor);
		}

		/*TIMELINE*/
		MovementTimeLine.PlayFromStart();
	}

	// Debug indicators.
	float elementCount =  static_cast<float>(indicators.Num());
	FString Message = FString::SanitizeFloat(elementCount);

	DebugMessage("Current spawned indicator count : " + Message, FColor::Cyan, 5.0f);

	for (int32 i = 0; i < indicators.Num(); ++i)
	{
		DebugMessage(FString::FromInt(i) + ": " + indicators[i]->GetActorLocation().ToString()
			, FColor::Cyan
			, 5.0f);
	}
}

// Called every frame
void APerpAlign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	/*TIMELINE*/
	MovementTimeLine.TickTimeline(DeltaTime);

}

FVector APerpAlign::GetRandomInitialLocation()
{
	Random.GenerateNewSeed();

	FVector RandomLocation = FVector(
		Random.RandRange(-500.0f, 500.0f),
		Random.RandRange(-500.0f, 500.0f),
		50);

	DebugMessage("New Random Location: " + RandomLocation.ToString(), FColor::Green, 5.0f);

	return  RandomLocation;
}

void APerpAlign::DebugMessage(FString Message, FColor Color, float Delay)
{
	if (IsDebug)
	{
		GEngine->AddOnScreenDebugMessage(-1, Delay, Color, Message);
	}
}

// Timeline functions from here.
void APerpAlign::InProgress()
{
	
}

void APerpAlign::OnFinished()
{
	
}


