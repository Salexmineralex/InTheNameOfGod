// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AsignNextPoint.h"

#include "InTheNameOfGod//Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_AsignNextPoint::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->AsignNextPoint();
	return EBTNodeResult::Succeeded;
}

FString UBTT_AsignNextPoint::GetStaticDescription()const
{
	return TEXT("Cacalcula el punto al que ir alrededor del player");
}