// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MoveToPlayerBTTaskNode.generated.h"

/**
 * 
 */
UCLASS()
class INTHENAMEOFGOD_API UMoveToPlayerBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)override;

	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	virtual FString GetStaticDescription()const override;

	
};
