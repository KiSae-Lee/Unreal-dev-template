# Unreal Development Template
## 1. Project Settings
### 1. *.uproject Settings
EnhancedInput plugin has been enabled.
```json
{
  "Plugins": [
    {
      "Name": "EnhancedInput",
      "Enabled": true
    }
  ]
}
```
### 2. *.Build.cs
EnhancedInput is ready for included.
```csharp
public Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore", 
			"EnhancedInput" // this is a plugin for controller.
		});
		
		PublicIncludePaths.AddRange(new string[]
		{
			"EnhancedInput/Public"
		});
```
## 2. Features
### 1. ATPSGameMode(C++)
This is an empty game mode for TPS observer.

**Spec:**</br>
Parent class - AGameModeBase

### 2. ATPSController(C++)
This is a basic controller for TPS observer. 

**Spec:**</br>
Parent class - AController</br>
Notation: _EnhancedInput_ must be enabled in project and also in build.

**Custom Method:**
1. Key mapping method for controller.
```c++
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
```

### 3. ATPSObserver(C++)
This is a basic TPS Pawn.

**Spec:**</br>
Parent class - APawn<

**Key Mapping:**
- MoveForward: W
- MoveBack: S
- MoveRight: D
- MoveLeft: A
- Up: LeftShift
- Down: LeftControl
- FreeFlyingMode: F (Toggle)

**Custom Method:**

Binding the movement.
```csharp
void ATPSObserver::Move(const FInputActionValue& ActionValue)
{
	FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();
	AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);
}
```
Binding the rotation.
```csharp
void ATPSObserver::Rotate(const FInputActionValue& ActionValue)
{
	FRotator Input(ActionValue[0], ActionValue[1], ActionValue[2]);
	Input *= GetWorld()->GetDeltaSeconds() * RotateScale;

	if (bFreeFly) { AddActorLocalRotation(Input); }
	else
	{
		Input += GetActorRotation();
		Input.Pitch = FMath::ClampAngle(Input.Pitch, -89.9f, 89.9f);
		Input.Roll = 0;
		SetActorRotation(Input);
	}
}
```
Changing flying mode.
```csharp
void ATPSObserver::ToggleFreeFly()
{
	bFreeFly = !bFreeFly;
}
```