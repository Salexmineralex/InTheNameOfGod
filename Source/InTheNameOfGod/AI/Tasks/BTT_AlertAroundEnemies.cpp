// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_AlertAroundEnemies.h"
#include "InTheNameOfGod//Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_AlertAroundEnemies::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->AlertSomeEnemies();
	return EBTNodeResult::Succeeded;
}

FString UBTT_AlertAroundEnemies::GetStaticDescription()const
{
	return TEXT("Avisa a lo jugadores cercanos");
}

