// Fill out your copyright notice in the Description page of Project Settings.


#include "KillEmAllGameMode.h"
# include "SImpleShooter/Characters/ShooterCharacter.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "SImpleShooter/Controllers/ShooterAIController.h"

void AKillEmAllGameMode::PawnKilled(APawn* PawnKilled) 
{
    Super::PawnKilled(PawnKilled);
    APlayerController* PlayerController = Cast<APlayerController>(PawnKilled->GetController());
    if (PlayerController != nullptr)
    {
        EndGame(false);
    } else 
    {
        for (AShooterAIController* Controller : TActorRange<AShooterAIController>(GetWorld()))
        {
            if (Controller->IsDead() == false) return;
        }
        EndGame(true);
    }
}

void AKillEmAllGameMode::EndGame(bool bIsPlayerWinner) 
{
    for (AController* Controller : TActorRange<AController>(GetWorld()))
    {
        bool bIsWinner = Controller->IsPlayerController() == bIsPlayerWinner;
        Controller->GameHasEnded(Controller->GetPawn(), bIsWinner);
    }
}
