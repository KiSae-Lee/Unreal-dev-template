// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "RandomWalker.generated.h"

UCLASS()
class PROJECT_API ARandomWalker : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARandomWalker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Vectors.
	FVector ActorInitialLocation;
	FVector TargetLocation;

	// Time line.
	FTimeline MovementTimeLine;
	FOnTimelineFloat ProgressFunction;
	FOnTimelineEvent OnTimelineFinishedFunction;

	// Boolean.
	bool bCalled;

	// Debug options.
	float TimeToDelay;
	
	// Function.
	UFUNCTION()
	void ProcessMovementTimeline(float Value);
	UFUNCTION()
	void OnEndMovementTimeLine();
	FVector GetRandomDestination();
	
public:
	// Custom codes.
	UPROPERTY(VisibleAnywhere, Category = "RandomWalker")
	USceneComponent* RootComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RandomWalker")
	UStaticMeshComponent* Mesh;

	// Controls random walker movement scale.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomWalker")
	int32 GridSize;
	
	// Random number generator.
	FRandomStream RandomStream;
	// Timer handle.
	FTimerHandle Timerhandle;
	
	// FloatCurve
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomWalker")
	UCurveFloat* CurveFloat;
};
