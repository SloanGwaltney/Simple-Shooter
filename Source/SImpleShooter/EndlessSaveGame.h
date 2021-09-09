// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "EndlessSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UEndlessSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	int32 GetNumberOfEnemiesKilled() const;
protected:
	UPROPERTY(BlueprintReadWrite)
	int32 NumberOfEnemiesKilled = 0;
};
