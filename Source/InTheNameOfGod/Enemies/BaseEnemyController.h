// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BaseEnemyController.generated.h"


class ACharacter;
class UBlackboard;
/**
 * 
 */
UCLASS()
class INTHENAMEOFGOD_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
		//virtual void BeginPlay()override ; se hace el begin play con blueprints

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta = (AllowPrivateAccess ="true"))
		TSoftObjectPtr<UBehaviorTree> behaviorTree{nullptr};

	ACharacter* target{ nullptr };

	int wayPointsAmount{ 0 };

public://tasks
	UFUNCTION(Category = Tasks)
	void UpdateNextTargetPoint();
	UFUNCTION(BlueprintCallable, Category = Tasks)
		EPathFollowingRequestResult::Type MoveToPlayer();
	UFUNCTION( Category = Tasks)
		void ResetBlackBoardKeyValue(FBlackboardKeySelector key);

	//Services
	UFUNCTION(Category = Services)
	void ChecknearbyEnemy();
	UFUNCTION(Category = Services)
	void SaveLastPlayerPosition();

	//protected:
	UFUNCTION(BlueprintCallable)
	void GetAllWayPoints();




};
