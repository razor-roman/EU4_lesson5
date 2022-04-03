// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "DamageTaker.h"
#include "HealthComponent.h"
#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/TargetPoint.h"
#include "GameFramework/Pawn.h"
#include "TankPawn.generated.h"

class UStaticMeshComponent;
class UCameraComponent;
class USpringArmComponent;
class ATankPlayerController;
class ACannon;

UCLASS()
class TANKOGEDDON_API ATankPawn : public APawn , public IDamageTaker
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATankPawn();
	ACannon * Cannon;
	
	FVector GetEyesPosition();

protected:
	//mesh
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Components")
	UStaticMeshComponent* BodyMesh;
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Components")
	UStaticMeshComponent* TurretMesh;
	//USpring
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Components")
	USpringArmComponent* SpringARm;
	//Camera
	UPROPERTY(VisibleDefaultsOnly,BlueprintReadWrite,Category="Components")
	UCameraComponent* Camera;
	//Cannon
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * CannonSetupPoint;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Cannon")
	TSubclassOf<ACannon> CannonClass;
	//health
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent * HealthComponent;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent * HitCollider; 
	//for AI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Patrol points" , Meta = (MakeEditWidget = true))
	TArray<ATargetPoint*> PatrollingPoints;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI|Move params|Accurency")
	float MovementAccurency = 500;
	//effects
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* HitEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	//var
	float MoveSpeed = 100;
	float TargetForwardAxisVaue;
	float PitchValue;
	float YawValue;
	float RollValue;
	UPROPERTY(EditInstanceOnly,BlueprintReadWrite,Category="Movement|Speed")
	float InterpolationKey = 0.1f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turret|Speed")
	float TurretRotationInterpolationKey = 0.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")
	float Health = 10;
	bool bCannon;
	float TargetForwardAxisValue;
	float TargetRightAxisValue;
	float CurrentRightAxisValue;

	UPROPERTY()
	ATankPlayerController* TankController;
	
	virtual void Destroyed() override;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//health
	UFUNCTION()
	void Die();

	UFUNCTION()
	void DamageTaked(float DamageValue);

	
public:
	UFUNCTION()
	void Fire();
	UFUNCTION()
	void FireSpecial();
	void SetupCannon();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION()
	void MoveForward(float AxisValue);
	void RotateLeft(float AxisValue);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void NewCannon(const TSubclassOf<ACannon>& CannonClass);
	//health
	UFUNCTION()
	void TakeDamage(FDamageData DamageData);
	//AI
	TArray<FVector> GetPatrollingPoints();
	void SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints);

	UFUNCTION()
	float GetMovementAccurency() {return MovementAccurency; };
	//ai
	UFUNCTION()
	FVector GetTurretForwardVector() ;
	UFUNCTION()
	void RotateTurretTo(FVector TargetPosition);

};
