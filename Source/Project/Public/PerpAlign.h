// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetMathLibrary.h"

#include "PerpAlign.generated.h"

UCLASS()
class PROJECT_API APerpAlign : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APerpAlign();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	// Actors.
	TArray<AActor*> indicators;
	
	// Vectors.
	TArray<FVector> Vertices;
	
	// Timeline components.
	FTimeline MovementTimeLine;
	FOnTimelineFloat InTimelineProgress;
	FOnTimelineEvent OnTimelineFinished;

	// Random.
	FRandomStream Random;

	// Vector functions.
	FVector GetRandomInitialLocation();

	// Timeline functions.
	UFUNCTION()
	void InProgress(float Value);
	UFUNCTION()
	void OnFinished();

	// PrepAlign function.
	void PerpAlign(AActor* RootActor, AActor* Actor);
	void ProcessPrepAlign();
	
	// Debug property.
	float DebugDelay;

	// Debug Message function.
	void DebugMessage(FString Message, FColor Color, float Delay);

public:
	// Flags.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerpAlign")
	bool IsDebug;
	
	// Timeline components.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerpAlignTimeline")
	UCurveFloat* CurveFloat;

	// Indicator.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="PerpAlign")
	TSubclassOf<AActor> Indicator;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="PerpAlign")
	int32 Count;
};
