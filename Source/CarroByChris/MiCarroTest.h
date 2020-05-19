// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "WheeledVehicle.h"
#include "MiCarroTest.generated.h"

/**
 * 
 */
class UPhysicallMaterial;
class UCameraComponent;
class USpringArmComponent;
class UInputComponent;
class UStaticlMeshComponent;
class USceneComponent;
UCLASS()
class CARROBYCHRIS_API AMiCarroTest : public AWheeledVehicle
{
	GENERATED_BODY()

public:
		UPROPERTY(EditAnywhere)
		USpringArmComponent* SpringArm;
		UPROPERTY(EditAnywhere)
		UCameraComponent* Camera;
		UPROPERTY(EditAnywhere)
		UStaticMeshComponent* volante;
public: 
	AMiCarroTest();
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void Tick(float Delta) override;		
	bool bInReverseGear;
	float angMax = 60.f;
	float roll, pitch;

protected:
	virtual void BeginPlay() override;
public:
	void MoveForward(float val);
	void UpdatePhysicsMaterial();
	void MoveRight(float val);
	void OnHandbrakePressed();
	void OnHandbrakeReleased();
	void RotarVolante(float val);
	void RegresoVolante();
	/** Returns SpringArm subobject **/
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	/** Returns Camera subobject **/
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
};
