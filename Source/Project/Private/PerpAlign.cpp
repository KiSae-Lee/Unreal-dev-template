// Fill out your copyright notice in the Description page of Project Settings.


#include "PerpAlign.h"

// Sets default values
APerpAlign::APerpAlign()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void APerpAlign::BeginPlay()
{
	Super::BeginPlay();

	if(!CurveFloat)
	{
		
	}
}

// Called every frame
void APerpAlign::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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


