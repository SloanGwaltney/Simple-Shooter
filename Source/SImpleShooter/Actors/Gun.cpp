// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "Components/AudioComponent.h"
#define OUT

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));

	Mesh->SetupAttachment(Root);
}

TriggerPullResult AGun::PullTrigger_Implementation(bool &bLineTraceHit, FHitResult &HitResult, FVector &ShotDirection)
{
	if (IsReloading()) return TriggerPullResult::Reloading;
	if (AmmoRoundsInClip < 1) {
		PlayDryFireSound();
		return TriggerPullResult::OutOfAmmo;
	}

	AmmoRoundsInClip = AmmoRoundsInClip - 1;
	SpawnMuzzleEffects();
	bLineTraceHit = GunTrace(HitResult, ShotDirection);

	if (bLineTraceHit) {
		SpawnHitEffects(HitResult, ShotDirection);
		DealDamage(HitResult, ShotDirection);
	}

	return TriggerPullResult::Fired;
}

void AGun::Reload() 
{
	bIsReloading = true;
	UAudioComponent* ReloadAudio = UGameplayStatics::SpawnSoundAttached(ReloadSound, Mesh, FName("MuzzleFlashSocket"));
	ReloadAudio->OnAudioFinished.AddDynamic(this, &AGun::ResetClipAmmo);
}

int AGun::GetRoundsInClip() const
{
	return AmmoRoundsInClip;
}

int AGun::GetMaxAmmoRoundsPerClip() const
{
	return MaxAmmoRoundsPerClip;
}

bool AGun::IsClipEmpty() const 
{
	return GetRoundsInClip() < 1;
}

bool AGun::IsReloading() const
{
	return bIsReloading;
}

bool AGun::IsGrabbable() const
{
	return bCanBeGrabbed;
}

void AGun::PlayDryFireSound() const
{
	UGameplayStatics::SpawnSoundAttached(OutOfAmmoTriggerPullSound, Mesh, FName("MuzzleFlashSocket"));
}

void AGun::SpawnMuzzleEffects() const
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, FName("MuzzleFlashSocket"));
}

float AGun::DealDamage(FHitResult &HitResult, FVector &ShotDirection) 
{
	FPointDamageEvent DamageEvent(Damage, HitResult, ShotDirection, nullptr);
	// Null check handled by GunTrace
	AController* OwnerController = GetOwnerController();
	if (HitResult.GetActor() != nullptr) return HitResult.GetActor()->TakeDamage(Damage, DamageEvent, OwnerController, this);
	return 0;
}

void AGun::SpawnHitEffects(FHitResult& Hit, FVector& ShotDirection) const
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, Hit.Location, ShotDirection.Rotation());
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, Hit.Location);
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	ResetClipAmmo();
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool AGun::GunTrace(FHitResult& Hit, FVector& ShotDirection) 
{	
	AController* OwnerController = GetOwnerController();
	if (OwnerController == nullptr) return false;

	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	OwnerController->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT PlayerViewPointRotation);

	ShotDirection = -PlayerViewPointRotation.Vector();
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * MaxRange;

	FCollisionQueryParams LineTraceQueryParams;
	LineTraceQueryParams.AddIgnoredActor(this);
	LineTraceQueryParams.AddIgnoredActor(GetOwner());

	FCollisionResponseParams LineTraceResponseParams;
	return GetWorld()->LineTraceSingleByChannel
	(
		Hit, 
		PlayerViewPointLocation, 
		LineTraceEnd, 
		ECollisionChannel::ECC_GameTraceChannel1,
		LineTraceQueryParams,
		LineTraceResponseParams
	);
}

void AGun::ResetClipAmmo() 
{
	AmmoRoundsInClip = MaxAmmoRoundsPerClip;
	bIsReloading = false;
}

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return nullptr;

	return OwnerPawn->GetController();
}

