// Fill out your copyright notice in the Description page of Project Settings.


#include "EndlessGameMode.h"
#include "GameFramework/Controller.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "SImpleShooter/Characters/ShooterCharacter.h"

void AEndlessGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        EndGame();
        return;
    }
    NumberOfEnemiesKilled = NumberOfEnemiesKilled + 1;
    NumberOfEnemiesRemaining = NumberOfEnemiesRemaining - 1;
    if (NumberOfEnemiesRemaining <= 0)
    {
        StartRound();
    }
    AShooterCharacter* CastedPawn = Cast<AShooterCharacter>(PawnKilled);
    CastedPawn->Destroy();
}

void AEndlessGameMode::StartPlay() 
{
    Super::StartPlay();
    StartRound();
}

int32 AEndlessGameMode::GetNumberOfSecondsUntilWaveStart() 
{
    return SecondsUntilWaveStart;
}

void AEndlessGameMode::EndGame() 
{
    UGameplayStatics::OpenLevel(GetWorld(), "WeaponSelectionLevel");
}

void AEndlessGameMode::StartRound() 
{
    WaveCounter = WaveCounter + 1;
    SecondsUntilWaveStart = 5;
    GetWorldTimerManager().SetTimer(WaveStartCountdownHandle, this, &AEndlessGameMode::WaveStartCountdownStep, 1.f, true);
}

void AEndlessGameMode::SpawnEnemies() 
{
    NumberOfEnemiesInWave = WaveCounter;
    NumberOfEnemiesRemaining = NumberOfEnemiesInWave;
    TArray<AActor*> SpawnPoints;
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), SpawnPoints);
    for (int32 x = 0; x < NumberOfEnemiesInWave; x++)
    {
        AActor* SpawnPointToUse = SpawnPoints[FMath::RandRange(0, SpawnPoints.Num() - 1)];
        GetWorld()->SpawnActor<AActor>(AIClassToSpawn, SpawnPointToUse->GetActorLocation(), SpawnPointToUse->GetActorRotation());
    }
}

void AEndlessGameMode::WaveStartCountdownStep() 
{
    SecondsUntilWaveStart = SecondsUntilWaveStart -1;
    if (SecondsUntilWaveStart <= 0)
    {
        GetWorldTimerManager().ClearTimer(WaveStartCountdownHandle);
        SpawnEnemies();
    }
}
