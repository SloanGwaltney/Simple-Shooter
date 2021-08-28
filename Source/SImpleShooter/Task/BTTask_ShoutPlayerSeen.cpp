// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ShoutPlayerSeen.h"
#include "SImpleShooter/Characters/ShooterCharacter.h"
#include "AIController.h"

EBTNodeResult::Type UBTTask_ShoutPlayerSeen::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    if (OwnerComp.GetAIOwner() == nullptr || OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;

    AShooterCharacter* AIShooter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AIShooter == nullptr) return EBTNodeResult::Failed;

    AIShooter->PlayerSeen();
    return EBTNodeResult::Succeeded;
}
