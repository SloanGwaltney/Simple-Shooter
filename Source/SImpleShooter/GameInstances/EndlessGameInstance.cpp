// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessGameInstance.h"


void UEndlessGameInstance::SetPlayerGunClass(UClass* GunClass) 
{
    PlayerGunClass = GunClass;
}

UClass* UEndlessGameInstance::GetPlayerGunClass() 
{
    return PlayerGunClass;
}
