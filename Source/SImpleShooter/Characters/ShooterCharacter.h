// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterCharacter.generated.h"

class AGun;
class USoundBase;

UCLASS()
class SIMPLESHOOTER_API AShooterCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AShooterCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool IsDead() const;

	UFUNCTION(BlueprintPure)
	float GetHealthPrecent() const;

	void Shoot();

	void Reload();

	void PlayerSeen();

	bool IsReloading();

	UFUNCTION(BlueprintPure)
	AGun* GetGun() const;

private:
	// Sensitivity of how much the character will look up and down in response to input
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationRateY = 10.f;

	// Sensitivity of how much the character will look left and right in response to input
	UPROPERTY(EditAnywhere, Category = "Movement")
	float RotationRateX = 10.f;

	// The class of the gun that the character will use
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGun> GunClass;

	UPROPERTY(EditAnywhere)
	USoundBase* HitScream;

	UPROPERTY(EditAnywhere)
	USoundBase* PlayerSeenCue;

	// The actual gun that the charecter will use
	UPROPERTY()
	AGun* Gun;

	// The Charecters current health
	float Health;

	// The max health the character can have
	UPROPERTY(EditAnywhere)
	float MaxHealth = 100.f;

	/**
	 *	This will move the actor forward.
	 *	@param AxisValue Value provided by the input controller that will serve to indicate how far forward the character should move .
	 *	If a negative value is provided 
	*/
	void MoveForward(float AxisValue);

	/**
	 *	This will move the actor to the right.
	 *	@param AxisValue Value provided by the input controller that will serve to indicate how far the character should move to the right.
	 *	If a negative value is provided  the character will move left instead
	*/
	void MoveRight(float AxisValue);

	/**
	 * This will make the actor look up.
	 * @param AxisValue Value provided by the input controller that will serve to indicate how far the character should look up.
	 * If a negative value is provided the character will look down
	*/
	void LookUpRate(float AxisValue);
	void LookRightRate(float AxisValue);
	void Jump();
	float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};
