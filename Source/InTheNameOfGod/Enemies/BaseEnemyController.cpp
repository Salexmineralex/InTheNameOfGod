// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyController.h"
//unreal
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "EngineUtils.h"

//project
#include "InTheNameOfGod/AI/WayPoint.h"

void ABaseEnemyController::BeginPlay()
{
	RunBehaviorTree(behaviorTree);

	TArray<AActor*> allWayPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),AWayPoint::StaticClass(), allWayPoints);
	wayPointsAmount = allWayPoints.Num();
	//GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, FString::Printf(TEXT("%d"), wayPointsAmount));
}

void ABaseEnemyController::UpdateNextTargetPoint()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	int32 idIndex = myBlackboard->GetValueAsInt("WayPointIndex");

	if (idIndex >= wayPointsAmount)
	{
		idIndex = 0;
		myBlackboard->SetValueAsInt("WayPointIndex", idIndex);
	}

	for (TActorIterator<AWayPoint> It(GetWorld()); It; ++It)
	{
		AWayPoint* currentWayPoint = *It;
		if (currentWayPoint->idPosition == idIndex)
		{
			myBlackboard->SetValueAsVector("WayPointPosition", currentWayPoint->GetActorLocation());
			break;
		}
	}
	myBlackboard->SetValueAsInt("WayPointIndex", idIndex + 1);
}

