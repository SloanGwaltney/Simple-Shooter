// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponCrate.h"
#include "SImpleShooter/Actors/Gun.h"

// Sets default values
AWeaponCrate::AWeaponCrate()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWeaponCrate::BeginPlay()
{
	Super::BeginPlay();
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	Gun->AttachToComponent(GetStaticMeshComponent(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

// Called every frame
void AWeaponCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

AGun* AWeaponCrate::GetGun() const
{
	return Gun;
}

