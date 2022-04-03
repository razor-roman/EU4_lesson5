// Fill out your copyright notice in the Description page of Project Settings.


#include "TankPawn.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankPlayerController.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/ArrowComponent.h"
#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

DECLARE_LOG_CATEGORY_EXTERN(TankLog, All, All);
DEFINE_LOG_CATEGORY(TankLog);


// Sets default values
ATankPawn::ATankPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//mesh
	BodyMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank Body"));		
	RootComponent=BodyMesh;
	//turret in 
	TurretMesh=CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tank turret"));
	TurretMesh->SetupAttachment(BodyMesh);
	//cannon setup to tank
	CannonSetupPoint = CreateDefaultSubobject<UArrowComponent>(TEXT("Cannon setup point"));
	CannonSetupPoint->AttachToComponent(TurretMesh, FAttachmentTransformRules::KeepRelativeTransform);
	//hit effect
	HitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	HitEffect->SetupAttachment(TurretMesh);
	//audio hit effect
	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
	AudioEffect->SetupAttachment(TurretMesh);
	//cam
	SpringARm=CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm"));
	SpringARm->SetupAttachment(BodyMesh);
	SpringARm->bDoCollisionTest=false;
	SpringARm->bInheritPitch=false;
	SpringARm->bInheritYaw=false;
	SpringARm->bInheritRoll=false;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringARm);
	//Rotate
	PitchValue=0.f;
	YawValue=0.f; 
	RollValue=0.f;
	// health
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health component"));
	HealthComponent->OnDie.AddUObject(this, &ATankPawn::Die);
	HealthComponent->OnDamaged.AddUObject(this, &ATankPawn::DamageTaked);
}

FVector ATankPawn::GetEyesPosition()
{
	return CannonSetupPoint->GetComponentLocation();

}

void ATankPawn::SetupCannon()
{
	if(Cannon)
	{
		Cannon->Destroy();		
	}
	FActorSpawnParameters params;
	params.Instigator = this;
	params.Owner = this;
	Cannon = GetWorld()->SpawnActor<ACannon>(CannonClass, params);
	Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

}

void ATankPawn::Destroyed()
{
	Super::Destroyed();
	if(Cannon)
	{
		Cannon->Destroy();
	}
		
}

// Called when the game starts or when spawned
void ATankPawn::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->AddHealth(Health);
	TankController = Cast<ATankPlayerController>(GetController());	
	SetupCannon();
}

void ATankPawn::Die()
{
	Destroy();
}

void ATankPawn::DamageTaked(float DamageValue)
{
	HitEffect->ActivateSystem();
	AudioEffect->Play();
	UE_LOG(TankLog, Warning, TEXT("Tank %s taked damage:%f Health:%f"), *GetName(), DamageValue, HealthComponent->GetHealth());
}

void ATankPawn::Fire()
{
	if(Cannon)
	{
		Cannon->Fire();
	}

}

void ATankPawn::FireSpecial()
{
	if(Cannon)
	{
		Cannon->FireSpecial();
	}
}


// Called every frame
void ATankPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FVector const currentLocation = GetActorLocation();
	FVector const forwardVector = GetActorForwardVector();
	FVector const movePosition = currentLocation + forwardVector * MoveSpeed *
	TargetForwardAxisVaue * DeltaTime;
	SetActorLocation(movePosition,true);	
	
	if(TankController)
	{
		FVector mousePos = TankController->GetMousePos();
		RotateTurretTo(mousePos);
	}
	
}

void ATankPawn::MoveForward(float AxisValue)
{
	TargetForwardAxisVaue = FMath::Lerp(TargetForwardAxisVaue, AxisValue, InterpolationKey);
}

void ATankPawn::RotateLeft(float AxisValue)
{
	FRotator const NewRotation = FRotator(PitchValue,YawValue,RollValue);
	//YawValue=AxisValue;
	YawValue= FMath::Lerp(YawValue, AxisValue, InterpolationKey);
	FQuat const QuatRotation = FQuat(NewRotation);	
	AddActorLocalRotation(QuatRotation,false,0,ETeleportType::None);
	
}


// Called to bind functionality to input
void ATankPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATankPawn::NewCannon(const TSubclassOf<ACannon>& CannonType)
{
	if(CannonType)
	{
		auto Transform = CannonSetupPoint->GetComponentTransform();
		Cannon = GetWorld()->SpawnActor<ACannon>(CannonType, Transform);
		Cannon->AttachToComponent(CannonSetupPoint, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}
}

void ATankPawn::TakeDamage(FDamageData DamageData)
{
	HealthComponent->TakeDamage(DamageData);
}

TArray<FVector> ATankPawn::GetPatrollingPoints()
{
  TArray<FVector> points;
  for (ATargetPoint* point: PatrollingPoints)
  {
     points.Add(point->GetActorLocation());
  }

  return points;
}

void ATankPawn::SetPatrollingPoints(TArray<ATargetPoint*> NewPatrollingPoints)
{
	PatrollingPoints = NewPatrollingPoints;
}


FVector ATankPawn::GetTurretForwardVector()
{
	return TurretMesh->GetForwardVector();
}

void ATankPawn::RotateTurretTo(FVector TargetPosition)
{
	FRotator targetRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), TargetPosition);
	FRotator currRotation = TurretMesh->GetComponentRotation();
	targetRotation.Pitch = currRotation.Pitch;
	targetRotation.Roll = currRotation.Roll;
	TurretMesh->SetWorldRotation(FMath::Lerp(currRotation, targetRotation, TurretRotationInterpolationKey));
}




