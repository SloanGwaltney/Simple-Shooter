// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Engine/Texture.h"
#include "CurrencyBase.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class SIMPLESHOOTER_API UCurrencyBase : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure)
	FName GetCurrencyName() const;

	UFUNCTION(BlueprintPure)
	FText GetDescription() const;

	UFUNCTION(BlueprintPure)
	UTexture* GetCurrencyIcon() const;

private:
	UPROPERTY(EditAnywhere)
	FName CurrencyName;

	UPROPERTY(EditAnywhere)
	FText Description;

	UPROPERTY(EditAnywhere)
	UTexture* CurrencyIcon;
};
