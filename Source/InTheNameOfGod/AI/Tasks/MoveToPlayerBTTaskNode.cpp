// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveToPlayerBTTaskNode.h"

#include "InThenameOfGod/Enemies/BaseEnemyController.h"


void UMoveToPlayerBTTaskNode::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ABaseEnemyController* control = Cast<ABaseEnemyController>(OwnerComp.GetOwner());
	EPathFollowingRequestResult::Type MoveToActorResult = control->MoveToPlayer();

	if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}

}

EBTNodeResult::Type UMoveToPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	bNotifyTick = 1;
	return EBTNodeResult::InProgress;


}

FString UMoveToPlayerBTTaskNode::GetStaticDescription()const
{
	return TEXT("Se mueve hacia el player");
}

