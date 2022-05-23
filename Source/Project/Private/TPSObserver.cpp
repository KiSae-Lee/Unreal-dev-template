// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSObserver.h"

// Sets default values
ATPSObserver::ATPSObserver()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set capsule.
	CylinderCapsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CylinderCapsule"));
	// Set arrow.
	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(CylinderCapsule);

	SprintArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SprintArm->SetupAttachment(CylinderCapsule);
	Cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Cam->SetupAttachment(SprintArm);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PawnMesh"));
	Mesh->SetupAttachment(CylinderCapsule);

	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingMovement"));
	MoveScale = 1.0f;
	RotateScale = 50.0f;
	bFreeFly = false;
}

// Called when the game starts or when spawned
void ATPSObserver::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATPSObserver::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATPSObserver::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Cast UEnhancedInputComponent as EIC.
	UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// Cast APCController as PCC;
	ATPSController* PCC = Cast<ATPSController>(Controller);
	// validity check.
	check(EIC && PCC);

	// Bind "MoveAction" action to EIC.
	EIC->BindAction(PCC->MoveAction, ETriggerEvent::Triggered, this, &ATPSObserver::Move);
	// Bind "RotateAction" action to EIC.
	EIC->BindAction(PCC->RotateAction, ETriggerEvent::Triggered, this, &ATPSObserver::Rotate);
	// Bind "FreeFlyAction" action to EIC.
	EIC->BindAction(PCC->FreeFlyAction, ETriggerEvent::Started, this, &ATPSObserver::ToggleFreeFly);

	// Get local player.
	ULocalPlayer* LocalPlayer = PCC->GetLocalPlayer();
	// validity check.
	check(LocalPlayer);
	// get subsystem of local player.
	UEnhancedInputLocalPlayerSubsystem* Subsystem =
		LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	// validity check.
	check(Subsystem);
	
	// clear mapping for local player.
	Subsystem->ClearAllMappings();
	// new control mapping for local player.
	Subsystem->AddMappingContext(PCC->PawnMappingContext, 0);
}

void ATPSObserver::Move(const FInputActionValue& ActionValue)
{
	FVector Input = ActionValue.Get<FInputActionValue::Axis3D>();
	// UFloatingPawnMovement handles scaling this input based on the DeltaTime for this frame.
	AddMovementInput(GetActorRotation().RotateVector(Input), MoveScale);
}

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

void ATPSObserver::ToggleFreeFly()
{
	bFreeFly = !bFreeFly;
}

