// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "SImpleShooter/Actors/Gun.h"
#include "Components/CapsuleComponent.h"
#include "SImpleShooter/SImpleShooterGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundBase.h"

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
	Gun = GetWorld()->SpawnActor<AGun>(GunClass);
	GetMesh()->HideBoneByName(TEXT("weapon_r"), EPhysBodyOp::PBO_None);
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("WeaponSocket"));
	Gun->SetOwner(this);
	Health = MaxHealth;
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
	Gun->PullTrigger();
}

void AShooterCharacter::Reload() 
{
	if (Gun == nullptr) return;
	Gun->Reload();
}

bool AShooterCharacter::IsReloading() 
{
	if (Gun == nullptr) return false;
	return Gun->IsReloading();
}

AGun* AShooterCharacter::GetGun() const
{
	if (Gun != nullptr) UE_LOG(LogTemp, Warning, TEXT("I GOT A GUN"));
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

bool AShooterCharacter::IsDead() const 
{
	return Health <= 0;
}

float AShooterCharacter::GetHealthPrecent() const
{
	return Health / MaxHealth;
}