// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "SImpleShooter/Actors/Gun.h"
#include "Components/CapsuleComponent.h"
#include "SImpleShooter/SImpleShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"
#include "SImpleShooter/Components/LevelChanger.h"
#include "SImpleShooter/GameInstances/EndlessGameInstance.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();
	Health = MaxHealth;
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	// TODO: Make a function
	if (GunClass)
	{
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
	}
	UEndlessGameInstance* GameInstance = GetWorld()->GetGameInstanceChecked<UEndlessGameInstance>();
	if (GameInstance != nullptr && Gun == nullptr)
	{
		UClass* WeaponClass = GameInstance->GetPlayerGunClass();
		if (WeaponClass == nullptr) return;
		UE_LOG(LogTemp, Warning, TEXT("DO I GET GUN FROM INSTANCE?"))
		Gun = GetWorld()->SpawnActor<AGun>(WeaponClass);
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
		Gun->SetOwner(this);
	}
}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Movement
	PlayerInputComponent->BindAxis(FName("MoveForward"), this, &AShooterCharacter::MoveForward);
	PlayerInputComponent->BindAxis(FName("LookUp"), this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis(FName("LookRight"), this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis(FName("MoveRight"), this, &AShooterCharacter::MoveRight);
	PlayerInputComponent->BindAction(FName("Jump"),  IE_Pressed, this, &AShooterCharacter::Jump);

	// Gun
	PlayerInputComponent->BindAction(FName("FireGun"), IE_Pressed, this, &AShooterCharacter::Shoot);
	PlayerInputComponent->BindAction(FName("Reload"), IE_Pressed, this, &AShooterCharacter::Reload);

	// Player Actions
	PlayerInputComponent->BindAction(FName("Grab"), IE_Pressed, this, &AShooterCharacter::GrabItem);

	// Analog Controller Specific Actions/Axis
	PlayerInputComponent->BindAxis(FName("LookUpRate"), this, &AShooterCharacter::LookUpRate);
	PlayerInputComponent->BindAxis(FName("LookRightRate"), this, &AShooterCharacter::LookRightRate);
}

void AShooterCharacter::MoveForward(float AxisValue) 
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

void AShooterCharacter::MoveRight(float AxisValue) 
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

// This is the same end result as APawn::AddControllerPitchInput but optimized for framerate-independence when using a analong controller 
void AShooterCharacter::LookUpRate(float AxisValue) 
{
	AddControllerPitchInput(AxisValue * RotationRateY * GetWorld()->GetDeltaSeconds());
}

// This is the same end result as &APawn::AddControllerYawInput but optimized for framerate-independence when using a analong controller 
void AShooterCharacter::LookRightRate(float AxisValue) 
{
	AddControllerYawInput(RotationRateX * AxisValue * GetWorld()->GetDeltaSeconds());
}

void AShooterCharacter::Jump() 
{
	// This prevents the user from creating overlaping jump actions by pressing the jump key mid jump
	if (IsJumpProvidingForce())
	{
		return;
	}
	Super::Jump();
}

void AShooterCharacter::Shoot() 
{
	if (Gun == nullptr) return;
	
	bool bLineTraceHit;
	FHitResult Hit;
	FVector ShotDirection;
	
	Gun->PullTrigger(bLineTraceHit, Hit, ShotDirection);
}

void AShooterCharacter::Reload() 
{
	if (Gun == nullptr) return;
	Gun->Reload();
}

/**
 * Actions performed when this player sees another player.
*/
void AShooterCharacter::PlayerSeen() 
{
	UGameplayStatics::SpawnSoundAtLocation(GetWorld(), PlayerSeenCue, GetActorLocation());
}

bool AShooterCharacter::IsReloading() 
{
	if (Gun == nullptr) return false;
	return Gun->IsReloading();
}

float AShooterCharacter::GetReach() const
{
	return Reach;
}

AGun* AShooterCharacter::GetGun() const
{
	return Gun;
}

float AShooterCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) 
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	DamageToApply = FMath::Min(Health, DamageToApply);
	Health = Health - DamageToApply;
	UE_LOG(LogTemp, Warning, TEXT("%f health left"), Health);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), HitScream, GetActorLocation());

	if (IsDead())
	{
		ASImpleShooterGameModeBase* GameMode = GetWorld()->GetAuthGameMode<ASImpleShooterGameModeBase>();
		if (GameMode != nullptr)
		{
			GameMode->PawnKilled(this);
		}
		DetachFromControllerPendingDestroy();
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	return DamageToApply;
}

void AShooterCharacter::GrabItem() 
{
	FHitResult Hit;
	bool bDidTraceHit = ReachLineTrace(Hit);
	AActor* HitActor = Hit.GetActor();
	if (bDidTraceHit == false || HitActor == nullptr) return;
	ULevelChanger* LevelChangeComponent = HitActor->FindComponentByClass<ULevelChanger>();
	if (LevelChangeComponent != nullptr)
	{
		UEndlessGameInstance* GameInstance = GetWorld()->GetGameInstanceChecked<UEndlessGameInstance>();
		if (GameInstance == nullptr) return;
		if (Gun != nullptr) GameInstance->SetPlayerGunClass(Gun->GetClass());
		LevelChangeComponent->ChangeLevel("Sandbox");
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Actor Hit: %s"), *HitActor->GetActorLabel())
	if (HitActor->GetClass()->IsChildOf(AGun::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("LOG TRACE HIT GUN"))
		AGun* GunToGrab = Cast<AGun>(HitActor);
		if (GunToGrab->IsGrabbable()) GrabGun(HitActor->GetClass());
		return;
	}
}

bool AShooterCharacter::ReachLineTrace(FHitResult &Hit) 
{	
	FVector ViewpointLocation;
	FRotator ViewpointRotation;
	GetController()->GetPlayerViewPoint(ViewpointLocation, ViewpointRotation);
	FVector LineTraceEnd = ViewpointLocation + ViewpointRotation.Vector() * Reach;
	return GetWorld()->LineTraceSingleByChannel(Hit, ViewpointLocation, LineTraceEnd, ECC_Visibility);
}

void AShooterCharacter::GrabGun(UClass* GunClassToGrab) 
{
	UE_LOG(LogTemp, Warning, TEXT("GRAB GUN"))
	Gun = GetWorld()->SpawnActor<AGun>(GunClassToGrab);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
}

bool AShooterCharacter::IsDead() const 
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPrecent() const
{
	return Health / MaxHealth;
}