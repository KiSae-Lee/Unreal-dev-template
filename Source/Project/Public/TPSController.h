// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "TPSController.generated.h"

UCLASS()
class PROJECT_API ATPSController : public APlayerController
{
	GENERATED_BODY()
	
	public:
	// Setup input actions and context mapping for player.
	virtual void SetupInputComponent() override;
	// Mapping context used for pawn control. 
	UPROPERTY()
	class UInputMappingContext* PawnMappingContext;
	// Action to update location.
	UPROPERTY()
	class UInputAction* MoveAction;
	// Action to update rotation.
	UPROPERTY()
	class UInputAction* RotateAction;
	// Action to toggle free fly mode.
	UPROPERTY()
	class UInputAction* FreeFlyAction;
};
