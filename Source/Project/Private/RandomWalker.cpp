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

	GridSize = 10;
	Speed = 1.0f;
	Delay = 2.0f;
}

// Called when the game starts or when spawned
void ARandomWalker::BeginPlay()
{
	Super::BeginPlay();

	GetWorld()->GetTimerManager().SetTimer(Timerhandle, this, &ARandomWalker::RandomDirection, Delay, true);
}

// Called every frame
void ARandomWalker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARandomWalker::RandomDirection()
{
	// debugging tests.
	FString direction;

	float randNum = RandomStream.FRand();

	if (randNum < 0.25f) { direction = "Forward"; }
	else if (randNum >= 0.25f && randNum < 0.5f) { direction = "Back"; }
	else if (randNum >= 0.5f && randNum < 0.75f) { direction = "Right"; }
	else { direction = "Left"; }

	// Debug message.
	GEngine->AddOnScreenDebugMessage(
		-1,
		1.0f,
		FColor::Green,
		direction);
}

