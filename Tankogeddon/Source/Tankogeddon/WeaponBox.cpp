// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBox.h"
#include "TankPawn.h"

// Sets default values
AWeaponBox::AWeaponBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	
	PrimaryActorTick.bCanEverTick = false;
	Collision=CreateDefaultSubobject<UBoxComponent>("Collision");
	RootComponent=Collision;
	Collision->OnComponentBeginOverlap.AddDynamic(this,&AWeaponBox::OnMeshOverlapBegin);

	Mesh=CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);
	
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBox::OnMeshOverlapBegin);
	Collision->SetCollisionProfileName(FName("OverlapAll"));
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Collision->SetGenerateOverlapEvents(true);

}

// Called when the game starts or when spawned
void AWeaponBox::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeaponBox::OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlap"));
	ATankPawn * playerPawn = Cast<ATankPawn>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if(OtherActor == playerPawn)
	{
		playerPawn->NewCannon(CannonClass);
		Destroy();
	}
}

// Called every frame
void AWeaponBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

