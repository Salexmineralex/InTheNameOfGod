// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_KeepingDistance.h"
#include "InThenameOfGod/Enemies/BaseEnemyController.h"


void UBTT_KeepingDistance::TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	ABaseEnemyController* control = Cast<ABaseEnemyController>(OwnerComp.GetOwner());
	control->KeepingDistance();
	//EPathFollowingRequestResult::Type MoveToActorResult = control->MoveToPlayer();
	//
	//if (MoveToActorResult == EPathFollowingRequestResult::AlreadyAtGoal)
	//{
	//	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//}

}

EBTNodeResult::Type UBTT_KeepingDistance::ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)
{
	bNotifyTick = 1;
	return EBTNodeResult::InProgress;


}

FString UBTT_KeepingDistance::GetStaticDescription()const
{
	return TEXT("Mantiene la distancia con el jugador");
}

