// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPlayerController.h"
#include "GameStructs.h"
#include "AITypes.h"
#include "TankPawn.h"
#include "DrawDebugHelpers.h"
#include "PhysXInterfaceWrapperCore.h"

void ATankPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	InputComponent->BindAxis("MoveForward",this,&ATankPlayerController::MoveForward);
	InputComponent->BindAxis("RotateLeft",this,&ATankPlayerController::RotateLeft);
	InputComponent->BindAction("Fire", IE_Pressed, this, &ATankPlayerController::Fire);
	InputComponent->BindAction("FireSpecial", IE_Pressed, this, &ATankPlayerController::FireSpecial);
	InputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &ATankPlayerController::SwitchWeapon);
}

void ATankPlayerController::Tick(float DeltaTime)
{
	FVector mouseDirection;
	DeprojectMousePositionToWorld(MousePos, mouseDirection);
	FVector pawnPos = TankPawn->GetActorLocation();
	MousePos.Z = pawnPos.Z;
	FVector dir = MousePos - pawnPos;
	dir.Normalize();
	MousePos= pawnPos+dir*1000;
	//DrawDebugLine(GetWorld(), pawnPos, MousePos, FColor::Green, false, 0.1f, 0, 5);
}

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TankPawn = Cast<ATankPawn>(GetPawn());
	
}

void ATankPlayerController::MoveForward(float AxisValue)
{
	TankPawn->MoveForward(AxisValue);
}

void ATankPlayerController::RotateLeft(float AxisValue)
{	
	TankPawn->RotateLeft(AxisValue);
}


void ATankPlayerController::Fire()
{
	TankPawn->Fire();
}

void ATankPlayerController::FireSpecial()
{
	TankPawn->FireSpecial();
}

void ATankPlayerController::SwitchWeapon()
{	
	TankCannon->SetCannonType();
}

ATankPlayerController::ATankPlayerController()
{
	bShowMouseCursor = true;

}


