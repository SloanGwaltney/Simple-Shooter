// Fill out your copyright notice in the Description page of Project Settings.


#include "CurrencyInventoryManager.h"

// Sets default values for this component's properties
UCurrencyInventoryManager::UCurrencyInventoryManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

bool UCurrencyInventoryManager::ProcessTransaction(FCurrencyInventoryItem Item) 
{
	if (Item.GetCurrency() == nullptr) return false;
	for (int i = 0; i < CurrencyInventory.Num(); i++)
	{
		if ( CurrencyInventory[i].GetCurrency()->GetCurrencyName() == Item.GetCurrency()->GetCurrencyName() )
		{
			if (CurrencyInventory[i].GetQuantity() >= Item.GetQuantity())
			{
				UE_LOG(LogTemp, Warning, TEXT("I do increment by: %f"), Item.GetQuantity())
				CurrencyInventory[i] + Item.GetQuantity();
				return true;
			}
			return false;
		}
		CurrencyInventory.Push(Item);
		return true;
	}
	CurrencyInventory.Push(Item);
	return true;
}

float UCurrencyInventoryManager::GetCurrencyAmount(FName CurrencyName) const 
{
	for (FCurrencyInventoryItem Item : CurrencyInventory)
	{
		if (Item.GetCurrency() != nullptr && Item.GetCurrency()->GetCurrencyName() == CurrencyName)
		{
			return Item.GetQuantity();
		}
	}
	return 0;
}


// Called when the game starts
void UCurrencyInventoryManager::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

// Called every frame
void UCurrencyInventoryManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

