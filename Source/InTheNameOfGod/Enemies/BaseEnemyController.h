// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"

#include "BaseEnemyController.generated.h"


class ACharacter;
class UBlackboard;
class UStaticMeshComponent;
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
	UPROPERTY(EditAnywhere,meta=(AllowPrivateAccess = "true"))
	UStaticMeshComponent* visionTrigger{ nullptr };

	USceneComponent* currentPointAroundPlayer{ nullptr };

	
	

public://tasks
	UFUNCTION(Category = Tasks)
	void UpdateNextTargetPoint();

	UFUNCTION(Category = Tasks)
	void ChangeState(int newState);

	UFUNCTION(BlueprintCallable, Category = Tasks)
	EPathFollowingRequestResult::Type MoveToPlayer();

	UFUNCTION(Category = Tasks)
	void AsignNextPoint();


	//Services
	UFUNCTION(Category = Services)
	void ChecknearbyEnemy();
	UFUNCTION(Category = Services)
	bool CheckCanSeePlayer();
	UFUNCTION(Category = Services)
	void SaveLastPlayerPosition();
	UFUNCTION(Category = Services)
	void UpdatePositionAroundPlayer();

	//protected:
	UFUNCTION(BlueprintCallable)
	void GetAllWayPoints();

	UFUNCTION(BlueprintCallable)
	void CPPBeginPlay();
	UFUNCTION(BlueprintCallable)
	void CPPBeginPlayPostBT();

	//variables funcionamiento
	bool hasCheckLastPlayerPosition{ true };

	int EnemyState{ 0 };//0:Patrol / 1: FollowPlayer / 2:GoingToFight / 3: WaitingForFight / 4: fighting
	
	




};
