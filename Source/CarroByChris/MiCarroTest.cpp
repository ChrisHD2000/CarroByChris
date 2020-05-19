// Fill out your copyright notice in the Description page of Project Settings.


#include "MiCarroTest.h"
#include "MyVehicleWheelFront.h"
#include "MyVehicleWheelRear.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/PoseableMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "Engine/SkeletalMesh.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/Engine.h"
#include "SkeletalRenderPublic.h"
#include "Components/SkinnedMeshComponent.h"

AMiCarroTest::AMiCarroTest() {

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CarMesh(TEXT("/Game/CarroPartes/CarroChasisEjesCorrectosEscalado.CarroChasisEjesCorrectosEscalado"));
	GetMesh()->SetSkeletalMesh(CarMesh.Object);	
	
	 
	// ponemos la cobertura
	
	UStaticMeshComponent* cobertura = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mi Cobertura"));
	cobertura->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Game/CarroPartes/Cobertura12.Cobertura12"));
	if (MeshAsset.Succeeded()) {
		cobertura->SetStaticMesh(MeshAsset.Object);
		cobertura->SetWorldScale3D(FVector(1.0f));
	}
	// creación y ubicación del volante 
	volante = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mi Volante"));
	volante->SetupAttachment(RootComponent);
	volante->SetRelativeLocation(FVector(10.f, 0.f, 55.f));
	volante->SetRelativeRotation(FRotator(-47.681f, 0.f, -90.f));

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset2(TEXT("/Game/CarroPartes/Volante/VolanteOrigen.VolanteOrigen"));
	if (MeshAsset2.Succeeded()) {
		volante->SetStaticMesh(MeshAsset2.Object);
		volante->SetWorldScale3D(FVector(1.0f));
	}
	roll = volante->GetRelativeRotation().Roll;
	pitch = volante->GetRelativeRotation().Pitch;
	//UE_LOG(LogTemp, Error, TEXT("ROLL ACTUAL: %f"), volante->GetRelativeRotation().Roll);

	//Añadimos el Blueprint de animación
	static ConstructorHelpers::FClassFinder<UObject> AnimBPClass(TEXT("/Game/CarroPartes/AnimFinal"));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetAnimInstanceClass(AnimBPClass.Class);

	
	//Creamos el vehículo 4w
	
	UWheeledVehicleMovementComponent4W* Vehicle4W = CastChecked<UWheeledVehicleMovementComponent4W>(GetVehicleMovement());
	check(Vehicle4W->WheelSetups.Num() == 4);
	// Setup the wheels
	Vehicle4W->WheelSetups[0].WheelClass = UMyVehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[0].BoneName = FName("PhysWheel_FL");
	Vehicle4W->WheelSetups[0].AdditionalOffset = FVector(0.f, -15.f, -1.f);

	Vehicle4W->WheelSetups[1].WheelClass = UMyVehicleWheelFront::StaticClass();
	Vehicle4W->WheelSetups[1].BoneName = FName("PhysWheel_FR");
	Vehicle4W->WheelSetups[1].AdditionalOffset = FVector(0.f, 15.f, -1.f);

	Vehicle4W->WheelSetups[2].WheelClass = UMyVehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[2].BoneName = FName("PhysWheel_BL");
	Vehicle4W->WheelSetups[2].AdditionalOffset = FVector(0.f, -17.f, -1.f);

	Vehicle4W->WheelSetups[3].WheelClass = UMyVehicleWheelRear::StaticClass();
	Vehicle4W->WheelSetups[3].BoneName = FName("PhysWheel_BR");
	Vehicle4W->WheelSetups[3].AdditionalOffset = FVector(00.f, 17.f, -1.f);

	// Adjust the tire loading
	Vehicle4W->MinNormalizedTireLoad = 0.0f;
	Vehicle4W->MinNormalizedTireLoadFiltered = 0.2f;
	Vehicle4W->MaxNormalizedTireLoad = 4.0f;
	Vehicle4W->MaxNormalizedTireLoadFiltered = 4.0f;

	// Engine 
	// Torque setup
	Vehicle4W->MaxEngineRPM = 12500.0f;
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->Reset();
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(0.0f, 6000.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(500.f, 9000.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12000.f, 9500.0f);
	Vehicle4W->EngineSetup.TorqueCurve.GetRichCurve()->AddKey(12500.f, 6000.f);

	


	// Adjust the steering 
	Vehicle4W->SteeringCurve.GetRichCurve()->Reset();
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(0.0f, 1.0f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(80.0f, 0.8f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(120.0f, 0.7f);
	Vehicle4W->SteeringCurve.GetRichCurve()->AddKey(300.0f, 0.1f);


	// Transmission	
	// We want Front Drive
	//Permite al carro poner su potencia hacia el frente, permitiendonos curvar con mayor rigidez
	Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_FrontDrive;
	//Vehicle4W->DifferentialSetup.DifferentialType = EVehicleDifferential4W::LimitedSlip_4W;

	// Drive the front wheels a little more than the rear
	Vehicle4W->DifferentialSetup.FrontRearSplit = 0.63f;

	// Automatic gearbox
	Vehicle4W->TransmissionSetup.bUseGearAutoBox = true;
	Vehicle4W->TransmissionSetup.GearSwitchTime = 0.15f;
	Vehicle4W->TransmissionSetup.GearAutoBoxLatency = 1.0f;

	UPrimitiveComponent* UpdatedPrimitive = Cast<UPrimitiveComponent>(Vehicle4W->UpdatedComponent);
	if (UpdatedPrimitive)
	{
		UpdatedPrimitive->BodyInstance.COMNudge = FVector(-25.f, 0.f, -16.f);
	}

	// Set the inertia scale. This controls how the mass of the vehicle is distributed.
	// Qué tan dificl es girar el carro respecto a los ejes x, y , z
	Vehicle4W->InertiaTensorScale = FVector(10.0f, 10.f, 1.2f * 1.7f);

	// Create a spring arm component for our chase camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetRelativeLocation(FVector(-300.f, 0.f, 180.f));
	SpringArm->SetWorldRotation(FRotator(-10.f, 0.0f, 0.0f));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 10.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = true;
	SpringArm->bInheritPitch = true;
	SpringArm->bInheritYaw = true;
	SpringArm->bInheritRoll = false;

	// Create the chase camera component 
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("ChaseCamera"));
	Camera->PostProcessSettings.MotionBlurAmount = 0.f;
	Camera->PostProcessSettings.MotionBlurMax = 0.f;
	Camera->PostProcessSettings.MotionBlurPerObjectSize = 0.f;

	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
	Camera->SetRelativeRotation(FRotator(0.0f, .0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
	Camera->FieldOfView = 90.f;

	
	

}
void AMiCarroTest::Tick(float Delta) {
	Super::Tick(Delta);
//	bInReverseGear = GetVehicleMovement()->GetCurrentGear < 0;
	

}

void AMiCarroTest::BeginPlay() {
	Super::BeginPlay();

}

void AMiCarroTest::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMiCarroTest::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMiCarroTest::MoveRight);
	PlayerInputComponent->BindAxis("RotateSteeringWheel", this, &AMiCarroTest::RotarVolante);
	PlayerInputComponent->BindAction("Handbrake", IE_Pressed, this, &AMiCarroTest::OnHandbrakePressed);
	PlayerInputComponent->BindAction("Handbrake", IE_Released, this, &AMiCarroTest::OnHandbrakeReleased);	
	PlayerInputComponent->BindAction("RetornoVolante", IE_Released, this, &AMiCarroTest::RegresoVolante);
	
}

void AMiCarroTest::MoveForward(float Val)
{
	
	GetVehicleMovementComponent()->SetThrottleInput(Val);

}

void AMiCarroTest::MoveRight(float Val)
{
	GetVehicleMovementComponent()->SetSteeringInput(Val);
}
void AMiCarroTest::OnHandbrakePressed()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(true);
}

void AMiCarroTest::OnHandbrakeReleased()
{
	GetVehicleMovementComponent()->SetHandbrakeInput(false);
}

// 
void AMiCarroTest::RotarVolante(float val) {
	/*UE_LOG(LogTemp, Warning, 
		TEXT("ROT ROLL: %f Lim inferior: %f Lim superio: %f"), 
			volante->GetRelativeRotation().Roll, 
			roll  - angMax,
			roll + angMax);*/
	// Necesitamos que el volante gire tanto como las llantas
	// en este caso roll es un numero negativo ya que se requirió para que el volante no quede desfasado
	if (volante->GetRelativeRotation().Roll  > (roll - angMax) &&
		volante->GetRelativeRotation().Roll  < (roll + angMax)) 
	{
			volante->SetRelativeRotation(volante->GetRelativeRotation() + FRotator(0.f, 0.f, 2.5f * val));
	}
	else {
		if (volante->GetRelativeRotation().Roll < (roll - angMax)) {
			volante->SetRelativeRotation(FRotator(pitch, 0.f, roll + 6.f - angMax));
		}
		if (volante->GetRelativeRotation().Roll > (roll + angMax)) {
			volante->SetRelativeRotation(FRotator(pitch, 0.f, roll - 6.f + angMax));
		}
	}

}

void AMiCarroTest::RegresoVolante() {
	volante->SetRelativeRotation(FRotator(pitch, 0.f, roll));
}
