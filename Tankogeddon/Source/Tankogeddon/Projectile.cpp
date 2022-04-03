// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "DamageTaker.h"
#include "TankPawn.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	USceneComponent * sceeneCpm = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = sceeneCpm;
	Collision = CreateDefaultSubobject<USphereComponent>("Collision");
	RootComponent=Collision;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AProjectile::OnMeshOverlapBegin);
	Collision->SetCollisionObjectType(ECollisionChannel::ECC_GameTraceChannel1);
	
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().SetTimer(MovementTimerHandle, this, &AProjectile::Move, MoveRate, true, MoveRate);
	PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();	
}

void AProjectile::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{	
	UE_LOG(LogTemp, Warning, TEXT("Projectile %s collided with %s. "), GetOwner(), *OtherActor->GetName());
	auto owner = GetOwner();
	
	AActor* ownerByOwner = owner != nullptr? owner->GetOwner(): nullptr;
	if(OtherActor != owner && OtherActor != ownerByOwner )
	{
		IDamageTaker * damageTakerActor = Cast<IDamageTaker>(OtherActor);
		if(damageTakerActor)
		{
			FDamageData damageData;
			damageData.DamageValue = Damage;
			damageData.Instigator = owner;
			damageData.DamageMaker = this;
			damageTakerActor->TakeDamage(damageData);
			UE_LOG(LogTemp, Warning, TEXT("Projectile %s damaged %s. "), *OtherActor->GetName());
	
		}
		else
		{
			OtherActor->Destroy();
		}
		Destroy();
		
	}	
}

void AProjectile::Move()
{
	FVector nextPosition = GetActorLocation() + GetActorForwardVector() * MoveSpeed * MoveRate;
	SetActorLocation(nextPosition,true);

}



