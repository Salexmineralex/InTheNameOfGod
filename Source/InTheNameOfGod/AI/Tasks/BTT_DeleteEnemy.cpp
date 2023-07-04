// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_DeleteEnemy.h"
#include "InTheNameOfGod/Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_DeleteEnemy::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->DeleteEnemy();
	return EBTNodeResult::Succeeded;
}

FString UBTT_DeleteEnemy::GetStaticDescription()const
{
	return TEXT("Elimina al jugador");
}

