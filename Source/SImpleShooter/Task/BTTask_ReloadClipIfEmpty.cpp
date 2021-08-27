// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ReloadClipIfEmpty.h"
#include "AIController.h"
#include "SImpleShooter/Characters/ShooterCharacter.h"
#include "SImpleShooter/Actors/Gun.h"

EBTNodeResult::Type UBTTask_ReloadClipIfEmpty::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    if (OwnerComp.GetAIOwner() == nullptr || OwnerComp.GetAIOwner()->GetPawn() == nullptr) return EBTNodeResult::Failed;
   
    AShooterCharacter* AICharacter = Cast<AShooterCharacter>(OwnerComp.GetAIOwner()->GetPawn());
    if (AICharacter == nullptr) return EBTNodeResult::Failed;

    AGun* AIPlayerGun = AICharacter->GetGun();

    if (AIPlayerGun == nullptr) return EBTNodeResult::Failed;

    // This should never happen but if it somehow did it is not a failure of this task but of something higher level
    if (AIPlayerGun->IsReloading()) return EBTNodeResult::Succeeded;

    if (AIPlayerGun->IsClipEmpty()) AIPlayerGun->Reload();

    return EBTNodeResult::Succeeded;
}
