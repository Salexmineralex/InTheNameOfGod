// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Uncover.h"

#include "InTheNameOfGod/Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_Uncover::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->Uncover();
	return EBTNodeResult::Succeeded;
}

FString UBTT_Uncover::GetStaticDescription()const
{
	return TEXT("Deja de leer el nodo de cubrirse");
}

