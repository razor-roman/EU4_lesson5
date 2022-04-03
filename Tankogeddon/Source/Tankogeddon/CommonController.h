// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Bp_CommonPawnClassPlayerTank.h"
#include "GameFramework/PlayerController.h"
#include "CommonController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ACommonController : public APlayerController
{
	GENERATED_BODY()
protected:
	void Fire();
	void FireSpecial();
	void SwitchWeapon();
	UPROPERTY()
	ABp_CommonPawnClassPlayerTank* PlayerTank;
	UPROPERTY()
	FVector MousePos;
public:
	ACommonController();
	virtual  void SetupInputComponent() override;
	virtual void Tick(float DeltaTime) override;
	FVector GetMousePos() { return MousePos; };

protected:
	virtual void BeginPlay() override;
	void MoveForward(float AxisValue);
	void RotateLeft(float AxisValue);
};
