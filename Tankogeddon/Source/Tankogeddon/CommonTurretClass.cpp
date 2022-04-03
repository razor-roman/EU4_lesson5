// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonTurretClass.h"

#include "Kismet/KismetMathLibrary.h"


ACommonTurretClass::ACommonTurretClass()
{
	
}

void ACommonTurretClass::BeginPlay()
{
	Super::BeginPlay();
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();	
	FTimerHandle _targetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ACommonTurretClass::Targeting, TargetingRate, true, TargetingRate);

}

void ACommonTurretClass::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void ACommonTurretClass::Destroyed()
{
	Super::Destroyed();
	if(Cannon)
		Cannon->Destroy();
}

void ACommonTurretClass::Targeting()
{
	if(IsPlayerInRange())
	{
		RotateToPlayer();
	}

	
	{
		Fire();
		//GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire();");
	}
}

void ACommonTurretClass::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));

}

bool ACommonTurretClass::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ACommonTurretClass::CanFire()
{
	FVector targetingDir = TurretMesh->GetForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
	return aimAngle <= Accurency;
}
