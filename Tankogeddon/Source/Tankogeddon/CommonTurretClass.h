// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonPawnClass.h"
#include "CommonTurretClass.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ACommonTurretClass : public ACommonPawnClass
{
	GENERATED_BODY()
public:
	ACommonTurretClass();
protected:
	virtual void BeginPlay() override;
	UPROPERTY()
	APawn * PlayerPawn;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRange = 1000;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingSpeed = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float TargetingRate = 0.005f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Targeting")
	float Accurency = 10;
public:
	virtual void Tick(float DeltaSeconds) override;
protected:
	virtual void Destroyed() override;
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerInRange();
	bool CanFire();
};
