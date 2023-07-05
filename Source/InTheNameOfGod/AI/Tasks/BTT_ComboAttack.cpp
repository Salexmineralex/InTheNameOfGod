// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ComboAttack.h"
#include "InTheNameOfGod/Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_ComboAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    ABaseEnemyController* control = Cast<ABaseEnemyController>(OwnerComp.GetOwner());
    control->ComboAttack();
    
    return EBTNodeResult::Succeeded;
}
