// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DestroyedEffect.generated.h"

UCLASS()
class TANKOGEDDON_API ADestroyedEffect : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADestroyedEffect();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	//effects
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UParticleSystemComponent* DestroyEffect;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "Components")
	UAudioComponent* AudioEffect;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
