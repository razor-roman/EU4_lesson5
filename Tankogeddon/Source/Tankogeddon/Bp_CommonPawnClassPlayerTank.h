// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Cannon.h"
#include "CoreMinimal.h"
#include "CommonPawnClass.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Bp_CommonPawnClassPlayerTank.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ABp_CommonPawnClassPlayerTank : public ACommonPawnClass
{
	GENERATED_BODY()
protected:
	//USpring
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Components")
	USpringArmComponent* SpringARm;
	//Camera
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Components")
	UCameraComponent* Camera;
	//var
	float PitchValue;
	float YawValue;
	float RollValue;
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="Movement|Speed")
	float InterpolationKey = 0.1f;	
	float TargetForwardAxisValue;
	float TargetRightAxisValue;
	float CurrentRightAxisValue;
public:
	UFUNCTION()
	void MoveForward(float AxisValue);
	void RotateLeft(float AxisValue);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	ABp_CommonPawnClassPlayerTank();
};
