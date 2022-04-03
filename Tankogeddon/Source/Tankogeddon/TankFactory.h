// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapLoader.h"
#include "TankPawn.h"
#include "Engine/TargetPoint.h"
#include "DamageTaker.h"
#include "GameFramework/Actor.h"
#include "TankFactory.generated.h"


UCLASS()
class TANKOGEDDON_API ATankFactory : public AActor, public IDamageTaker

{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	AMapLoader* LinkedMapLoader;
	
public:	
	// Sets default values for this actor's properties
	ATankFactory();
	UFUNCTION()
	void TakeDamage(FDamageData DamageData);

protected:
	bool isActivated=true;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnNewTank();
 
	UFUNCTION()
	 void Die();

	UFUNCTION()
	 void DamageTaked(float DamageValue);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Health")	
	float Health=5;
protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UStaticMeshComponent* BuildingMesh;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UArrowComponent * TankSpawnPoint;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UBoxComponent * HitCollider;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UHealthComponent * HealthComponent;

	//effects
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* SpawnEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* HitEffect;	
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* HitAudioEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* SpawnAudioEffect;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TSubclassOf<ATankPawn> SpawnTankClass;
 
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Spawn tanks params")
	float SpawnTankRate = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn tanks params")
	TArray<ATargetPoint*> TankWayPoints;

	const FString BodyMeshPath ="StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'";

};
