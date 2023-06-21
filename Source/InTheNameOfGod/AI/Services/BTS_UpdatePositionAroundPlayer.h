// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTS_UpdatePositionAroundPlayer.generated.h"

/**
 * 
 */
UCLASS()
class INTHENAMEOFGOD_API UBTS_UpdatePositionAroundPlayer : public UBTService
{
	GENERATED_BODY()
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)override;

	
};
