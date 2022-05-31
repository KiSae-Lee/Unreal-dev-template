// Fill out your copyright notice in the Description page of Project Settings.


#include "PerpAlign.h"

// Sets default values
APerpAlign::APerpAlign()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	IsDebug = true;
	Count = 7;
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
	else if(!InitLocation)
	{
		DebugMessage("Please add InitLocation!", FColor::Red, 5.0f);
	}
	else
	{
		/*TIMELINE*/
		// Setup debug delay.
		DebugDelay = CurveFloat->FloatCurve.GetLastKey().Value;
		DebugMessage("DebugDelay : " + FString::SanitizeFloat(DebugDelay), FColor::Green, 5.0f);
		
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
			AActor* SpawnedInidicator;
			AActor* SpawnedInitLocation;
			Vertices.Add(GetRandomInitialLocation());
			SpawnedInidicator = GetWorld()->SpawnActor(Indicator);
			SpawnedInidicator->SetActorLocation(Vertices[i]);
			indicators.Add(SpawnedInidicator);

			SpawnedInitLocation = GetWorld()->SpawnActor(InitLocation);
			SpawnedInitLocation->SetActorLocation(Vertices[i]);
			initLocations.Add(SpawnedInitLocation);
		}
		
		/*TIMELINE*/
		MovementTimeLine.PlayFromStart();
	}

	/*INDICATORS DEBUG*/
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

	for (int i = 0; i < indicators.Num() - 1; ++i)
	{
		DrawDebugLine(
			GetWorld(),
			indicators[i]->GetActorLocation(),
			indicators[i + 1]->GetActorLocation(),
			FColor::Red
			);
	}

	for (int i = 0; i < initLocations.Num() - 1; ++i)
	{
		DrawDebugLine(
			GetWorld(),
			initLocations[i]->GetActorLocation(),
			initLocations[i + 1]->GetActorLocation(),
			FColor::Blue
			);
	}
	

}

FVector APerpAlign::GetRandomInitialLocation()
{
	Random.GenerateNewSeed();

	FVector RandomLocation = FVector(
		Random.RandRange(-500.0f, 500.0f),
		Random.RandRange(-500.0f, 500.0f),
		Random.RandRange(0.0f, 300.0f));

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

FVector APerpAlign::PerpAlign(AActor* RootActor, AActor* Actor)
{
	FVector rootPos = RootActor->GetActorLocation();
	FVector pos = Actor->GetActorLocation();

	double AbsX = UKismetMathLibrary::Abs(rootPos.X - pos.X);
	double AbsY = UKismetMathLibrary::Abs(rootPos.Y - pos.Y);
	double AbsZ = UKismetMathLibrary::Abs(rootPos.Z - pos.Z);
	
	double x;
	double y;
	double z;
	if(AbsX > AbsY)
	{
		y =  pos.Y - rootPos.Y;
		if(AbsX > AbsZ)
		{
			x = 0;
			z = pos.Z - rootPos.Z;
		}
		else
		{
			x = pos.X - rootPos.X;
			z = 0;
		}
	}
	else
	{
		x = pos.X - rootPos.X;
		if(AbsY > AbsZ)
		{
			y = 0;
			z = pos.Z - rootPos.Z;
		}
		else
		{
			y = pos.Y - rootPos.Y;
			z = 0;
		}
	}

	FVector newLocation = FVector(x, y, z);
	return newLocation;
}

TArray<FVector> APerpAlign::ProcessPrepAlign()
{
	TArray<FVector> result;
	result.Add(FVector(0,0,0));
	
	for (int i = 0; i < indicators.Num() - 2; ++i)
	{
		result.Add(PerpAlign(indicators[i], indicators[i+1]));
	}
	result.Add(FVector(0,0,0));

	return result;
}

// Timeline functions from here.
void APerpAlign::InProgress(float Value)
{
	TArray<FVector> TargetLocations = ProcessPrepAlign();
	for (int i = 0; i < indicators.Num(); ++i)
	{
		FVector NewLocation = FMath::Lerp(
			indicators[i]->GetActorLocation(),
			indicators[i]->GetActorLocation() - TargetLocations[i],
			Value);
		
		indicators[i]->SetActorLocation(NewLocation);
	}
}

void APerpAlign::OnFinished()
{
	// MovementTimeLine.PlayFromStart();
}


