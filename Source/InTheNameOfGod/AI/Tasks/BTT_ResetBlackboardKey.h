// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "BTT_ResetBlackboardKey.generated.h"

/**
 * 
 */
UCLASS()
class INTHENAMEOFGOD_API UBTT_ResetBlackboardKey : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Blackboard")
	FBlackboardKeySelector BlackboardKey;

	//UFUNCTION(CATEGORY = Task)
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& ownerComp, uint8* NodeMemory)override;

	virtual FString GetStaticDescription()const override;


};
