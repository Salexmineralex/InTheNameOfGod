// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/Services/BTS_CheckCombatDistance.h"

#include "InThenameOfGod/Enemies/BaseEnemyController.h"

void UBTS_CheckCombatDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	ABaseEnemyController* control = Cast<ABaseEnemyController>(OwnerComp.GetOwner());
	control->CheckCombatDistance();

}