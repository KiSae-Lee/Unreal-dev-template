// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSController.h"

// Map key to action for mapping context with option modifiers.
static void MapKey(
	UInputMappingContext* InputMappingContext,
	UInputAction* InputAction,
	FKey Key,
	bool bNegate = false,
	bool bSwizzle = false,
	EInputAxisSwizzle SwizzleOrder = EInputAxisSwizzle::YXZ)
{
	FEnhancedActionKeyMapping& Mapping = InputMappingContext->MapKey(InputAction, Key);
	UObject* Outer = InputMappingContext->GetOuter();

	if(bNegate)
	{
		UInputModifierNegate* Negate = NewObject<UInputModifierNegate>(Outer);
		Mapping.Modifiers.Add(Negate);
	}
	if(bSwizzle)
	{
		UInputModifierSwizzleAxis* Swizzle = NewObject<UInputModifierSwizzleAxis>(Outer);
		Swizzle->Order = SwizzleOrder;
		Mapping.Modifiers.Add(Swizzle);
	}
}

void ATPSController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Create these objects here and not in constructor because we only need them on the client.
	PawnMappingContext = NewObject<UInputMappingContext>(this);

	// MOVEMENT:
	MoveAction = NewObject<UInputAction>(this);
	MoveAction->ValueType = EInputActionValueType::Axis3D;

	MapKey(PawnMappingContext, MoveAction, EKeys::W);
	MapKey(PawnMappingContext, MoveAction, EKeys::S, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::A, true, true);
	MapKey(PawnMappingContext, MoveAction, EKeys::D, false, true);

	MapKey(PawnMappingContext, MoveAction, EKeys::LeftShift, false, true, EInputAxisSwizzle::ZYX);
	MapKey(PawnMappingContext, MoveAction, EKeys::LeftControl, true, true, EInputAxisSwizzle::ZYX);

	// ROTATION:
	RotateAction = NewObject<UInputAction>(this);
	RotateAction->ValueType = EInputActionValueType::Axis3D;
	MapKey(PawnMappingContext, RotateAction, EKeys::MouseY);
	MapKey(PawnMappingContext, RotateAction, EKeys::MouseX, false,true);
	MapKey(PawnMappingContext, RotateAction, EKeys::Q, true,true, EInputAxisSwizzle::ZYX);
	MapKey(PawnMappingContext, RotateAction, EKeys::E, false,true, EInputAxisSwizzle::ZYX);

	FreeFlyAction = NewObject<UInputAction>(this);
	MapKey(PawnMappingContext, FreeFlyAction, EKeys::F);
}

