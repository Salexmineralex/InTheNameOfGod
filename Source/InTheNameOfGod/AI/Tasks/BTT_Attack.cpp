// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Attack.h"
#include "InTheNameOfGod//Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_Attack::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->Attack();
	return EBTNodeResult::Succeeded;
}

FString UBTT_Attack::GetStaticDescription()const
{
	return TEXT("Ataca");
}

