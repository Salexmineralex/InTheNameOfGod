// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_Cover.h"
#include "InTheNameOfGod//Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_Cover::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->Cover();
	return EBTNodeResult::Succeeded;
}

FString UBTT_Cover::GetStaticDescription()const
{
	return TEXT("Se protege de un ataque");
}

