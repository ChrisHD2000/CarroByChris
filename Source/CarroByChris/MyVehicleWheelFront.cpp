// Fill out your copyright notice in the Description page of Project Settings.


#include "MyVehicleWheelFront.h"
#include "TireConfig.h"
#include "UObject/ConstructorHelpers.h"
UMyVehicleWheelFront::UMyVehicleWheelFront() {
	ShapeRadius = 19.911f;
	ShapeWidth = 17.332f ;
	bAffectedByHandbrake = false;
	SteerAngle = 60.f;

	// Fuerzas de suspension
	//R = 19.911 CM
	// ANCHO = 17.332 CM
	SuspensionForceOffset = -2.0f;
	SuspensionMaxRaise = 8.0f;
	SuspensionMaxDrop = 6.0f;
	// La frecuencia natural es la frecuencia en la cual un sistema tiende a oscilar 
//en la ausencia de cualquier fuerza externa durante el manejo (los amortiguadores)
	SuspensionNaturalFrequency = 1.f;
	//Determina cómo oscilan las llantas del vehículo después de una alteración durante su camino
	SuspensionDampingRatio = 30.5f;

	//Ajustes mientras el carro gira o arrancha
	LatStiffMaxLoad = 10.3f;
	LatStiffValue = 50.f;
	LongStiffValue = 5000.f;

	static ConstructorHelpers::FObjectFinder<UTireConfig> TireData(TEXT("/Game/MyMesh/TireConfig/Front.Front"));
	TireConfig = TireData.Object;

}
