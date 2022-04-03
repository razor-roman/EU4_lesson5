// Fill out your copyright notice in the Description page of Project Settings.


#include "Turret.h"

#include "DestroyedEffect.h"
#include "DrawDebugHelpers.h"
#include "PhysXInterfaceWrapperCore.h"
#include "Components/AudioComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ATurretMethod::ATurretMethod()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret body"));
	RootComponent = BodyMesh;

	TurretMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Turret turret"));
	TurretMesh->AttachToComponent(BodyMesh, FAttachmentTransformRules::KeepRelativeTransform);

	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);

	HitCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("Hit collider"));
	HitCollider->SetupAttachment(TurretMesh);
	
	UStaticMesh * turretMeshTemp = LoadObject<UStaticMesh>(this, *TurretMeshPath);
	if(turretMeshTemp)
		TurretMesh->SetStaticMesh(turretMeshTemp);
	
	UStaticMesh * bodyMeshTemp = LoadObject<UStaticMesh>(this, *BodyMeshPath);
	if(bodyMeshTemp)
		BodyMesh->SetStaticMesh(bodyMeshTemp);

	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	HitEffect->SetupAttachment(TurretMesh);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
	AudioEffect->SetupAttachment(TurretMesh);
	
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATurretMethod::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATurretMethod::DamageTaked);

}

// Called when the game starts or when spawned
void ATurretMethod::BeginPlay()
{
	Super::BeginPlay();
	FActorSpawnParameters params;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Cannon->CountPlus(100);
	HealthComponent->AddHealth(5);
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();	
	
	FTimerHandle _targetingTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(_targetingTimerHandle, this, &ATurretMethod::Targeting, TargetingRate, true, TargetingRate);

}

// Called every frame
void ATurretMethod::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATurretMethod::Destroyed()
{
	ADestroyedEffect* destroy = GetWorld()->SpawnActor<ADestroyedEffect>(DestroyedClass, CannonSetupPoint->GetComponentLocation(), CannonSetupPoint->GetComponentRotation());
			
	if(Cannon)
		Cannon->Destroy();
	
}

void ATurretMethod::Targeting()
{
	if(IsPlayerInRange())
	{
		RotateToPlayer();
	}

	if(CanFire() && Cannon && Cannon->IsReadyToFire())
	{
		Fire();
	}
}

void ATurretMethod::RotateToPlayer()
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), PlayerPawn->GetActorLocation());
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TargetingSpeed));
}

bool ATurretMethod::IsPlayerInRange()
{
	return FVector::Distance(PlayerPawn->GetActorLocation(), GetActorLocation()) <= TargetingRange;
}

bool ATurretMethod::CanFire()
{
	if(!IsPlayerSeen())
		return false;
	FVector targetingDir = TurretMesh->GetForwardVector();
	FVector dirToPlayer = PlayerPawn->GetActorLocation() - GetActorLocation();
	dirToPlayer.Normalize();
	float aimAngle = FMath::RadiansToDegrees(acosf(FVector::DotProduct(targetingDir, dirToPlayer)));
	return aimAngle <= Accurency;

}

void ATurretMethod::Fire()
{
	if(Cannon)
		Cannon->Fire();

}

bool ATurretMethod::IsPlayerSeen()
{
	FVector playerPos = PlayerPawn->GetActorLocation();
	FVector eyesPos =  TurretMesh->GetComponentLocation();
	FHitResult hitResult;
	FCollisionQueryParams traceParams = FCollisionQueryParams(FName(TEXT("FireTrace")), true, this);
	traceParams.bTraceComplex = true;
	traceParams.AddIgnoredActor(this);
	traceParams.bReturnPhysicalMaterial = false;

	if(GetWorld()->LineTraceSingleByChannel(hitResult, eyesPos, playerPos, ECollisionChannel::ECC_Visibility, traceParams))
	{
	     
		if(hitResult.Actor.Get())
		{
			DrawDebugLine(GetWorld(), eyesPos, hitResult.Location, FColor::Cyan, false, 0.5f, 0, 10);
			return hitResult.Actor.Get() == PlayerPawn;
		}
	}
	DrawDebugLine(GetWorld(), eyesPos, playerPos, FColor::Cyan, false, 0.5f, 0, 10);
	return false;
}

void ATurretMethod::Die()
{
	Destroy();
}

void ATurretMethod::DamageTaked(float DamageValue)
{
	UE_LOG(LogTemp, Warning, TEXT("Turret %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
	HitEffect->ActivateSystem();
	AudioEffect->Play();
	
}

void ATurretMethod::TakeDamage(FDamageData DamageData)
{
	
	HealthComponent->TakeDamage(DamageData);
}
