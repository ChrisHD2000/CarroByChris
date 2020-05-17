// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVehicleWheelRear.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"

UMyVehicleWheelRear::UMyVehicleWheelRear() {

	ShapeRadius = 22.97f ;
	ShapeWidth = 20.07f ;
	bAffectedByHandbrake = true;
	SteerAngle = 0.f;

	// Fuerzas de suspension
	//R = 22.97 CM
	// ANCHO = 20.07 cm 
	SuspensionForceOffset = -2.0f;
	SuspensionMaxRaise = 8.0f;
	SuspensionMaxDrop = 6.0f;
	// La frecuencia natural es la frecuencia en la cual un sistema tiende a oscilar 
	//en la ausencia de cualquier fuerza externa durante el manejo (los amortiguadores)
	
	SuspensionNaturalFrequency = 1.f;
	//Determina cómo oscilan las llantas del vehículo después de una alteración durante su camino
	SuspensionDampingRatio = 30.5f;


	// Aumentar un poco la velocidad
	LatStiffMaxLoad = 10.3f;
	LatStiffValue = 50.f;
	LongStiffValue = 5000.f;


	// Find the tire object and set the data for it

	static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/MyMesh/TireConfig/Rear.Rear"));
	TireConfig = TireData.Object;



}

