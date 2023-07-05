// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTT_AlertAroundEnemies.generated.h"

/**
 * 
 */
UCLASS()
class INTHENAMEOFGOD_API UBTT_AlertAroundEnemies : public UBTTaskNode
{
	GENERATED_BODY()
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory);
	virtual FString GetStaticDescription()const override;
	
};
