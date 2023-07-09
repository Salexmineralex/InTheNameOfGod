// Fill out your copyright notice in the Description page of Project Settings.


#include "BTS_CanEnemyCombat.h"

void UBTS_CanEnemyCombat::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ABaseEnemyController* control = Cast<ABaseEnemyController>(OwnerComp.GetOwner());
	control->CheckEnemyCanCombat();

}