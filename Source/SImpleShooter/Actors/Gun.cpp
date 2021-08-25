// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
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

void AGun::PullTrigger() 
{
	UGameplayStatics::SpawnEmitterAttached(MuzzleFlash, Mesh, FName("MuzzleFlashSocket"));
	UGameplayStatics::SpawnSoundAttached(MuzzleSound, Mesh, FName("MuzzleFlashSocket"));

	FVector ShotDirection;
	FHitResult LineTraceHitResult;

	bool bLineTraceHit = GunTrace(LineTraceHitResult, ShotDirection);

	if (bLineTraceHit)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, LineTraceHitResult.Location, ShotDirection.Rotation());
		UGameplayStatics::SpawnSoundAtLocation(GetWorld(), HitSound, LineTraceHitResult.Location);
		AActor* HitActor = LineTraceHitResult.GetActor();
		if (HitActor != nullptr)
		{
			FPointDamageEvent DamageEvent(Damage, LineTraceHitResult, ShotDirection, nullptr);
			// Null check handled by GunTrace
			AController* OwnerController = GetOwnerController();
			HitActor->TakeDamage(Damage, DamageEvent, OwnerController, this);
		}
	}
}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
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

AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!OwnerPawn) return nullptr;

	return OwnerPawn->GetController();
}

