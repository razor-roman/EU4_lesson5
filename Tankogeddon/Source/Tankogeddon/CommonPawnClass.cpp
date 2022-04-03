// Fill out your copyright notice in the Description page of Project Settings.


#include "CommonPawnClass.h"

// Sets default values
ACommonPawnClass::ACommonPawnClass()
{
	
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));		
	RootComponent=BodyMesh;
	//turret in 
	TurretMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);
	//cannon setup to tank
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);
	// health
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ACommonPawnClass::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ACommonPawnClass::DamageTaked);
}

void ACommonPawnClass::TakeDamage(FDamageData DamageData)
{	
	HealthComponent->TakeDamage(DamageData);
}

// Called when the game starts or when spawned
void ACommonPawnClass::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACommonPawnClass::Destroyed()
{
	Super::Destroyed();
	if(Cannon)
	{
		Cannon->Destroy();
	}
}

void ACommonPawnClass::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}
}

void ACommonPawnClass::Die()
{
	Destroy();
}

void ACommonPawnClass::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void ACommonPawnClass::SetupCannon()
{
	if(Cannon)
	{
		Cannon->Destroy();		
	}
	FActorSpawnParameters params;
	//params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

// Called every frame
void ACommonPawnClass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ACommonPawnClass::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

