// Fill out your copyright notice in the Description page of Project Settings.


#include "Bp_CommonPawnClassPlayerTank.h"
void ABp_CommonPawnClassPlayerTank::MoveForward(float AxisValue)
{
	TargetForwardAxisValue = FMath::Lerp(TargetForwardAxisValue, AxisValue, InterpolationKey);
}

void ABp_CommonPawnClassPlayerTank::RotateLeft(float AxisValue)
{
	FRotator const NewRotation = FRotator(PitchValue,YawValue,RollValue);
	//YawValue=AxisValue;
	YawValue= FMath::Lerp(YawValue, AxisValue, InterpolationKey);
	FQuat const QuatRotation = FQuat(NewRotation);	
	AddActorLocalRotation(QuatRotation,false,0,ETeleportType::None);
	
}

void ABp_CommonPawnClassPlayerTank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

ABp_CommonPawnClassPlayerTank::ABp_CommonPawnClassPlayerTank()
{
	//cam
	SpringARm=CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringARm->SetupAttachment(BodyMesh);
	SpringARm->bDoCollisionTest=false;
	SpringARm->bInheritPitch=false;
	SpringARm->bInheritYaw=false;
	SpringARm->bInheritRoll=false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringARm);
	//Rotate
	PitchValue=0.f;
	YawValue=0.f; 
	RollValue=0.f;
}
