// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CouldBeCombo.h"
#include "InTheNameOfGod//Enemies/BaseEnemyController.h"


EBTNodeResult::Type UBTT_CouldBeCombo::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	control->CalculateRandomPercent();
	return EBTNodeResult::Succeeded;
}

FString UBTT_CouldBeCombo::GetStaticDescription()const
{
	return TEXT("Calcula un random para hacer un ataque o un combo");
}

