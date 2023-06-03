// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ResetBlackboardKey.h"


#include "InThenameOfGod/Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_ResetBlackboardKey::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->ResetBlackBoardKeyValue(BlackboardKey);
	
	return EBTNodeResult::Succeeded;
}

FString UBTT_ResetBlackboardKey::GetStaticDescription()const
{
	return TEXT("Reseta el valor de una variable");
}

