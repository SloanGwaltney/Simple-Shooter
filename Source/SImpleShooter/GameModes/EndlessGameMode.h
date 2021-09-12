// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SImpleShooter/SImpleShooterGameModeBase.h"
#include "SImpleShooter/Characters/ShooterCharacter.h"
#include "SImpleShooter/Currencies/CurrencyBase.h"
#include "EndlessGameMode.generated.h"

class UEndlessSaveGame;
/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API AEndlessGameMode : public ASImpleShooterGameModeBase
{
	GENERATED_BODY()
public:

	AEndlessGameMode();
	virtual void PawnKilled(APawn* PawnKilled) override;
	virtual void StartPlay();

	UFUNCTION(BlueprintPure)
	int32 GetNumberOfSecondsUntilWaveStart();

	UFUNCTION(BlueprintPure)
	int32 GetNumberOfEnemiesKilled() const;

	UFUNCTION(BlueprintPure)
	UCurrencyBase* GetCurrencyRewarded();
protected:
	UPROPERTY(BlueprintReadWrite)
	int32 NumberOfEnemiesKilled = 0;
private:
	void EndGame();
	void StartRound();
	void SpawnEnemies();
	void WaveStartCountdownStep();

	int32 NumberOfEnemiesInWave = 1;
	int32 NumberOfEnemiesRemaining = 0;
	int32 SecondsUntilWaveStart = 0;
	int32 WaveCounter = 0;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AShooterCharacter> AIClassToSpawn;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCurrencyBase> CurrencyRewardedClass;

	UCurrencyBase* CurrencyRewarded;

	FTimerHandle WaveStartCountdownHandle;
};
