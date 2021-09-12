// Fill out your copyright notice in the Description page of Project Settings.

#include "CurrencyBase.h"
#include "Engine/Texture.h"

FName UCurrencyBase::GetCurrencyName() const
{
    return CurrencyName;    
}

FText UCurrencyBase::GetDescription() const
{
    return Description;
}

UTexture* UCurrencyBase::GetCurrencyIcon() const
{
    return CurrencyIcon;
}
