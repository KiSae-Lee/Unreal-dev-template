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
	TArray<FVector> VisitedLocation;

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
	void ShowIndicator();
	void UpdateVisitedLocation();

	// Debug.
	void ShowDebugScreenMessage(FString Message, FColor Color);
	
public:
	// flag.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomWalker")
	bool bShowIndicator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomWalker")
	bool bDebug = true;
	// Properties.
	UPROPERTY(VisibleAnywhere, Category = "RandomWalker")
	USceneComponent* RootComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "RandomWalker")
	UStaticMeshComponent* Mesh;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomWalker")
	TSubclassOf<class AActor> IndicatorBP;

	// Controls random walker movement scale.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomWalker")
	int32 GridSize;
	
	// Random number generator.
	FRandomStream RandomStream;
	
	// FloatCurve
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "RandomWalker")
	UCurveFloat* CurveFloat;
};
