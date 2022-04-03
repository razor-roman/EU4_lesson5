// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonController.h"

void ACommonController::Fire()
{
	PlayerTank->Fire();
}

void ACommonController::FireSpecial()
{
//	PlayerTank->FireSpecial();
}

void ACommonController::SwitchWeapon()
{
	PlayerTank->Cannon->SetCannonType();
}

ACommonController::ACommonController()
{
	bShowMouseCursor = true;
}

void ACommonController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward",this,&ACommonController::MoveForward);
	InputComponent->BindAxis("RotateLeft",this,&ACommonController::RotateLeft);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ACommonController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ACommonController::FireSpecial);
	InputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ACommonController::SwitchWeapon);
	
}

void ACommonController::Tick(float DeltaTime)
{
	FVector mouseDirection;
	DeprojectMousePositionToWorld(MousePos, mouseDirection);
	FVector pawnPos = PlayerTank->GetActorLocation();
	MousePos.Z = pawnPos.Z;
	FVector dir = MousePos - pawnPos;
	dir.Normalize();
	MousePos= pawnPos+dir*1000;
	//DrawDebugLine(GetWorld(), pawnPos, MousePos, FColor::Green, false, 0.1f, 0, 5);
}

void ACommonController::BeginPlay()
{
	Super::BeginPlay();
	PlayerTank = Cast<ABp_CommonPawnClassPlayerTank>(GetPawn());
}

void ACommonController::MoveForward(float AxisValue)
{
	//PlayerTank->MoveForward(AxisValue);
	
}

void ACommonController::RotateLeft(float AxisValue)
{
	PlayerTank->RotateLeft(AxisValue);
}

