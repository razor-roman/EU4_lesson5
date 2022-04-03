// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

class ACannon;
class ATankPawn;
/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankPlayerController : public APlayerController
{
	GENERATED_BODY()
protected:
	void Fire();
	void FireSpecial();
	void SwitchWeapon();
	UPROPERTY()
	ATankPawn* TankPawn;
	ACannon* TankCannon;
	UPROPERTY()
	FVector MousePos;

public:
	ATankPlayerController();
	virtual  void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	FVector GetMousePos() { return MousePos; };

protected:
	virtual void BeginPlay() override;
	void MoveForward(float AxisValue);
	void RotateLeft(float AxisValue);
};
