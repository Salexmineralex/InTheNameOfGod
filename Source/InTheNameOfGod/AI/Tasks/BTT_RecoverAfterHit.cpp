// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_RecoverAfterHit.h"

#include "InTheNameOfGod/Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_RecoverAfterHit::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->RecoverAfterHit();
	return EBTNodeResult::Succeeded;
}

FString UBTT_RecoverAfterHit::GetStaticDescription()const
{
	return TEXT("Deja de leer el nodo de ser golpeado");
}