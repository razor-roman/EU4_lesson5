// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "AmmoBox.generated.h"

UCLASS()
class TANKOGEDDON_API AAmmoBox : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoBox();

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	UBoxComponent* Collision;
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category="Components")
	UStaticMeshComponent* Mesh;
	//var
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Components")
	float Count;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnMeshOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
