// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Gun.generated.h"

UCLASS()
class SIMPLESHOOTER_API AGun : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGun();
	void PullTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	// The skeletal mesh of the gun
	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* Mesh;

	// The muzzle flash particle effect the gun will use when fireing
	UPROPERTY(EditAnywhere)
	UParticleSystem* MuzzleFlash;

	// The particle effect that plays when the guns projectile hits a target
	UPROPERTY(EditAnywhere)
	UParticleSystem* HitEffect;

	UPROPERTY(EditAnywhere)
	USoundBase* MuzzleSound;

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	// The max range the weapons projectile can go in centimeters
	UPROPERTY(EditAnywhere)
	float MaxRange = 1000.f;

	// The amount of damage the gun will deal to it's hit target
	UPROPERTY(EditAnywhere)
	float Damage = 25.f;

	bool GunTrace(FHitResult& Hit, FVector& ShotDirection);
	AController* GetOwnerController() const;
};
