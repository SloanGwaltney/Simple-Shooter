// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelChanger.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
ULevelChanger::ULevelChanger()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void ULevelChanger::ChangeLevel(FName LevelName) 
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}


// Called when the game starts
void ULevelChanger::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void ULevelChanger::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

