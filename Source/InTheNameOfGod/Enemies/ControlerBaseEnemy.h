// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ControlerBaseEnemy.generated.h"

/**
 * 
 */

class ACharacter;

UCLASS()
class INTHENAMEOFGOD_API AControlerBaseEnemy : public AAIController
{
	GENERATED_BODY()
		//virtual void BeginPlay()override;

private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		UBehaviorTree* behaviorTree {
		nullptr
	};

	ACharacter* target{ nullptr };

	int wayPointsAmount{ 0 };
	
};
