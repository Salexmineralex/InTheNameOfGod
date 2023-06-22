// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlerBaseEnemy.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "EngineUtils.h"

#include "InTheNameOfGod/AI/WayPoint.h"


/*void AControlerBaseEnemy::BeginPlay()
{

	TArray<AActor*> allWayPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), allWayPoints);
	wayPointsAmount = allWayPoints.Num();

	//RunBehaviorTree(behaviorTree);
	//UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	//FVector a(211.f, 169.f, 39.f);
	//myBlackboard->SetValueAsVector("WayPointPosition", a);

	//MoveTo(a);


}*/