// Fill out your copyright notice in the Description page of Project Settings.


#include "DestroyedEffect.h"

#include "Components/AudioComponent.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ADestroyedEffect::ADestroyedEffect()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DestroyEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShootEffect"));
	DestroyEffect->SetupAttachment(RootComponent);

	AudioEffect = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioEffect"));
	AudioEffect->SetupAttachment(DestroyEffect);
}

// Called when the game starts or when spawned
void ADestroyedEffect::BeginPlay()
{
	Super::BeginPlay();
	DestroyEffect->ActivateSystem();
	AudioEffect->Play();
	SetLifeSpan(2);
}

// Called every frame
void ADestroyedEffect::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

