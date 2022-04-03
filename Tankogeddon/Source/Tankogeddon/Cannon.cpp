// Fill out your copyright notice in the Description page of Project Settings.


#include "Cannon.h"
#include "DrawDebugHelpers.h"
#include "DamageTaker.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "TimerManager.h"
#include "Chaos/ChaosDebugDraw.h"
#include "Components/AudioComponent.h"
#include "Engine/Engine.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ACannon::ACannon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent * sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cannon mesh"));
	Mesh->SetupAttachment(RootComponent);

	ProjectileSpawnPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Spawn point"));
	ProjectileSpawnPoint->SetupAttachment(Mesh);
	
	ShootEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	ShootEffect->SetupAttachment(ProjectileSpawnPoint);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
	AudioEffect->SetupAttachment(Mesh);
	
}

void ACannon::Fire()
{
	
	if(!ReadyToFire)
	{
		return;	
	}
	ReadyToFire = false;
	switch (Type)
	{
	case ECannonType::FireProjectile:
		{
			if(ProjectileCount==0)
			{
				GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Out of Ammo");
				return;
			}
			ShootEffect->ActivateSystem();
			AudioEffect->Play();
			if(GetOwner() && GetOwner() == GetWorld()->GetFirstPlayerController()->GetPawn())
			{
				if(ShootForceEffect)
				{
					FForceFeedbackParameters shootForceEffectParams;
					shootForceEffectParams.bLooping = false;
					shootForceEffectParams.Tag = "shootForceEffectParams";
					GetWorld()->GetFirstPlayerController()->ClientPlayForceFeedback(ShootForceEffect, shootForceEffectParams);
				}
				if(ShootShake)
				{
					GetWorld()->GetFirstPlayerController()->ClientStartCameraShake(ShootShake);
				}
			}
			ProjectileCount--;
			UE_LOG(LogTemp, Warning, TEXT("ProjectileCount= %f "), ProjectileCount);
			AProjectile* projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileClass, ProjectileSpawnPoint->GetComponentLocation(), ProjectileSpawnPoint->GetComponentRotation());
			break;	
		}
		
	case ECannonType::FireTrace:
		{
			UE_LOG(LogTemp, Warning, TEXT("ProjectileCount= %f "), ProjectileCount);
			GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire - trace");
			ProjectileCount--;
			FHitResult Result;
			FCollisionObjectQueryParams Params;
			Params.AddObjectTypesToQuery(ECollisionChannel::ECC_Vehicle);
			Params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);		
			Params.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldStatic);
			auto Start = ProjectileSpawnPoint->GetComponentLocation();
			auto End=Start+ProjectileSpawnPoint->GetForwardVector()*FireRange;
			bool HasHIt = GetWorld()->LineTraceSingleByObjectType(
				Result,
				Start,
				End,
				 Params);
			if (HasHIt)
			{				
				IDamageTaker * damageTakerActor = Cast<IDamageTaker>(Result.Actor);
				if(Result.Actor.Get())
				{
					FDamageData damageData;
					damageData.DamageValue = DamageTrace;
					damageData.Instigator = this;
					damageData.DamageMaker = this;
					damageTakerActor->TakeDamage(damageData);			
				}
				End=Result.Location;
			}
			DrawDebugLine(GetWorld(),Start,End,FColor::Green,false,1,0.1,10);
			//if(Result.Actor.IsValid()) Result.Actor->Destroy();
			break;	
		}
		
	}		
	GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);

}

void ACannon::FireSpecial()
{
	if(ProjectileCount==0) GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Out of Ammo");
	if(!ReadyToFire)
	{
		return;	
	}
	ReadyToFire = false;
	if(Type == ECannonType::FireProjectile && ProjectileCount!=0)
	{				
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::TripleShot, 1 / FireRate/3, true,0);
		ProjectileCount=ProjectileCount-3;
		//UE_LOG(LogTemp, Warning, TEXT("ProjectileCount= %f "), ProjectileCount);
	}	
}

bool ACannon::IsReadyToFire()
{
	return ReadyToFire;

}

void ACannon::TripleShot()
{	
	GEngine->AddOnScreenDebugMessage(10, 1,FColor::Green, "Fire - special");
	UE_LOG(LogTemp, Warning, TEXT("Count = %f "), MultiShot);
	MultiShot--;
	if(MultiShot==0)
	{
		GetWorld()->GetTimerManager().ClearTimer(ReloadTimerHandle);
		MultiShot=3;
		GetWorld()->GetTimerManager().SetTimer(ReloadTimerHandle, this, &ACannon::Reload, 1 / FireRate, false);	
	}
	
}


// Called when the game starts or when spawned
void ACannon::BeginPlay()
{
	Super::BeginPlay();
	Reload();

}

void ACannon::Reload()
{
	ReadyToFire = true;	
}

void ACannon::CountPlus(float Amount)
{
	ProjectileCount=ProjectileCount+Amount;
}

void ACannon::SetCannonType()
{
	UE_LOG(LogTemp, Warning, TEXT("Switch"));
	/*if(Type==ECannonType::FireProjectile)
	{
		Type=ECannonType::FireTrace;
	}
	else
	{
		Type=ECannonType::FireProjectile;
	}*/
}

// Called every frame
void ACannon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}


