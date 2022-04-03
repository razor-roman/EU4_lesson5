// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Cannon.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "WeaponBox.generated.h"

UCLASS()
class TANKOGEDDON_API AWeaponBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponBox();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	UBoxComponent* Collision;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Ammo")
	TSubclassOf<ACannon> CannonClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="Ammo Params")
	bool bInfinite =false;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
