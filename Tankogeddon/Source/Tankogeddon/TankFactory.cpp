// Fill out your copyright notice in the Description page of Project Settings.


#include "TankFactory.h"

#include "MapLoader.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATankFactory::ATankFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	USceneComponent * sceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceneComp;
	BuildingMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Building Mesh"));
	BuildingMesh->SetupAttachment(sceneComp);
	TankSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	TankSpawnPoint->AttachToComponent(sceneComp, FAttachmentTransformRules::KeepRelativeTransform);
	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(sceneComp);
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankFactory::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankFactory::DamageTaked);
	//HIT EFFECTS
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("HitEffect"));
	HitEffect->SetupAttachment(BuildingMesh);
	HitAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("HitAudioEffect"));
	HitAudioEffect->SetupAttachment(BuildingMesh);
	//SPAWN EFFECTS
	SpawnEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SpawnEffect"));
	SpawnEffect->SetupAttachment(BuildingMesh);
	SpawnAudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("SpawnAudioEffect"));
	SpawnAudioEffect->SetupAttachment(BuildingMesh);
}

void ATankFactory::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

// Called when the game starts or when spawned
void ATankFactory::BeginPlay()
{
	HealthComponent->AddHealth(Health);
	Super::BeginPlay();
	if(LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(false);
	FTimerHandle _targetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATankFactory::SpawnNewTank, SpawnTankRate, true, SpawnTankRate);
	
}

void ATankFactory::SpawnNewTank()
{
	if(!isActivated) return;
	FTransform spawnTransform(TankSpawnPoint->GetComponentRotation(), TankSpawnPoint->GetComponentLocation(), FVector(1));
	ATankPawn * newTank = GetWorld()->SpawnActorDeferred<ATankPawn>(SpawnTankClass, spawnTransform, this, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	//
	newTank->SetPatrollingPoints(TankWayPoints);
	//
	UGameplayStatics::FinishSpawningActor(newTank, spawnTransform);
	SpawnEffect->ActivateSystem();
	SpawnAudioEffect->Play();
}

void ATankFactory::Die()
{
	if(LinkedMapLoader)
		LinkedMapLoader->SetIsActivated(true);
	isActivated=false;
	UStaticMesh * bodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if(bodyMeshTemp)
		BuildingMesh->SetStaticMesh(bodyMeshTemp);
	HitEffect->ActivateSystem();
	HitAudioEffect->Play();
}

void ATankFactory::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Factory %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
	HitEffect->ActivateSystem();
	HitAudioEffect->Play();
}

// Called every frame
void ATankFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

