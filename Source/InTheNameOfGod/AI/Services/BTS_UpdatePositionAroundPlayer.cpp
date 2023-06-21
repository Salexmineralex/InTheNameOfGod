// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_UpdatePositionAroundPlayer.h"

#include "InThenameOfGod/Enemies/BaseEnemyController.h"

void UBTS_UpdatePositionAroundPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ABaseEnemyController* control = Cast<ABaseEnemyController>(OwnerComp.GetOwner());
	control->UpdatePositionAroundPlayer();

}
