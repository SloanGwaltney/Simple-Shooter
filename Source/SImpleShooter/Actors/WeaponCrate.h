// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SImpleShooter/Actors/Gun.h"
#include "Engine/StaticMeshActor.h"
#include "WeaponCrate.generated.h"

UCLASS()
class SIMPLESHOOTER_API AWeaponCrate : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponCrate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintPure)
	AGun* GetGun() const;
private:	
	AGun* Gun;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AGun> GunClass;
};
