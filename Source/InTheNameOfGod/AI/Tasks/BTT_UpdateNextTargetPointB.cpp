// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_UpdateNextTargetPointB.h"

#include "InTheNameOfGod//Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_UpdateNextTargetPointB::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* controller = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	controller->UpdateNextTargetPoint();

	return EBTNodeResult::Succeeded;
}

FString UBTT_UpdateNextTargetPointB::GetStaticDescription()const
{
	return TEXT("Actualiza el siguiente wayPoint");
}