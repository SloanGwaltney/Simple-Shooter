// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "EndlessGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIMPLESHOOTER_API UEndlessGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	void SetPlayerGunClass(UClass* PlayerGunClass);
	UClass* GetPlayerGunClass();
private:
	UClass* PlayerGunClass;
};
