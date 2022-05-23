// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/CapsuleComponent.h"

#include "GameFramework/Pawn.h"
// Engine headers.
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
// Custom controller.
#include "TPSController.h"
// EnhancedInput plugin headers.
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
// MUST GO TO THE LAST.
#include "TPSObserver.generated.h"

UCLASS()
class PROJECT_API ATPSObserver : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATPSObserver();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// PAWN PROPERTIES:
	// Capsule and Arrow settings.
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Observer")
	UCapsuleComponent* CylinderCapsule;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Observer")
	UArrowComponent* Arrow;
	// Camera Settings
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Observer")
	USpringArmComponent* SprintArm;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Category = "Observer")
	UCameraComponent* Cam;
	// Mesh settings.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Observer")
	UStaticMeshComponent* Mesh;
	
	// MOVEMENT SETTING:
	// Handle input to update location.
	void Move(const struct FInputActionValue& ActionValue);
	// Movement speed.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ObserverMovement")
	float MoveScale;
	
	// ROTATION SETTING:
	// Handle input to update rotation.
	void Rotate(const struct FInputActionValue& ActionValue);
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ObserverMovement")
	float RotateScale;

	// MODE SETTING:
	// Handle input to enable free flying.
	void ToggleFreeFly();
	// Whether to use free flying mode. Caution: might cause motion sickness!
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "ObserverMovement")
	uint32 bFreeFly:1;
	
	// Use floating pawn movement to smooth out motion.
	UPROPERTY(BlueprintReadOnly, EditAnywhere, Category = "ObserverMovement")
	class UFloatingPawnMovement* Movement;


};
