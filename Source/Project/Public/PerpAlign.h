// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Actor.h"
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
	// Timeline components.
	FTimeline MovementTimeLine;
	FOnTimelineFloat InTimelineProgress;
	FOnTimelineEvent OnTimelineFinished;

	// Timeline functions.
	void InProgress();
	void OnFinished();

public:
	// Timeline components.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PerpAlignTimeline")
	UCurveFloat* CurveFloat;

	// Indicator.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category ="PerpAlign")
	TSubclassOf<AActor> Indicator;
};
