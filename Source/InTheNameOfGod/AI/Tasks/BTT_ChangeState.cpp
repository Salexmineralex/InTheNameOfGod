// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChangeState.h"

#include "InTheNameOfGod//Enemies/BaseEnemyController.h"

EBTNodeResult::Type UBTT_ChangeState::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	ABaseEnemyController* controller = Cast<ABaseEnemyController>(ownerComp.GetOwner());
	controller->ChangeState(newState);



	return EBTNodeResult::Succeeded;
}

FString UBTT_ChangeState::GetStaticDescription()const
{
	return TEXT("Cambia el estado(0:Patrol / 1: FollowPlayer / 2:GoingToFight / 3: WaitingForFight / 4: fighting)");
}

