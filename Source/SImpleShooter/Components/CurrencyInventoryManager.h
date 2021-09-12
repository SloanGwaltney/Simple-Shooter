// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SImpleShooter/Currencies/CurrencyBase.h"
#include "CurrencyInventoryManager.generated.h"


USTRUCT()
struct FCurrencyInventoryItem 
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UCurrencyBase* Currency;

	UPROPERTY()
	float Quantity;

public:
	FCurrencyInventoryItem()
	{
		Currency = nullptr;
		Quantity = 0.f;
	}

	FCurrencyInventoryItem(UCurrencyBase* Curr)
	{
		Currency = Curr;
		Quantity = 0.f;
	}

	FCurrencyInventoryItem(UCurrencyBase* Curr, float Quant)
	{
		UE_LOG(LogTemp, Warning, TEXT("I DO CALL THE RIGHT CONSTRUCTOR"))
		Currency = Curr;
		Quantity = Quant;
	}

	FCurrencyInventoryItem* operator+(float Amount)
	{
		Quantity = Quantity + Amount;
		return this;
	}

	FCurrencyInventoryItem* operator-(float Amount)
	{
		Quantity = Quantity - Amount;
		return this;
	}

	UCurrencyBase* GetCurrency() const
	{
		return Currency;
	}

	float GetQuantity() const
	{
		return Quantity;
	}
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SIMPLESHOOTER_API UCurrencyInventoryManager : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCurrencyInventoryManager();

	bool ProcessTransaction(FCurrencyInventoryItem Item);

	UFUNCTION(BlueprintPure)
	float GetCurrencyAmount(FName CurrencyName) const;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY()
	TArray<FCurrencyInventoryItem> CurrencyInventory;		
};
