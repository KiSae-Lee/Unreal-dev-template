// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
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
	
	// Custom codes.
	// PROPERTIES:
	UPROPERTY(VisibleAnywhere, Category = "Moveer")
	USceneComponent* RootComp;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mover")
	UStaticMeshComponent* Mesh;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mover")
	int32 GridSize;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mover")
	float Speed;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Mover")
	float Delay;
	
	// Random number generator.
	FRandomStream RandomStream;
	// Timer handle.
	FTimerHandle Timerhandle;

	// Function.
	void RandomDirection();
};
