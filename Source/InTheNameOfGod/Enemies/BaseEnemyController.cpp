// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyController.h"
//unreal
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"


//project
#include "InTheNameOfGod/AI/WayPoint.h"

void ABaseEnemyController::GetAllWayPoints()
{
	TArray<AActor*> allWayPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), allWayPoints);
	wayPointsAmount = allWayPoints.Num();
}




void ABaseEnemyController::UpdateNextTargetPoint()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	int32 idIndex = myBlackboard->GetValueAsInt("WayPointIndex");

	if (!hasCheckLastPlayerPosition)
	{
		hasCheckLastPlayerPosition = true;
		FVector newPos = myBlackboard->GetValueAsVector("LastPlayerPosKnown");
		myBlackboard->SetValueAsVector("WayPointPosition",newPos);
		myBlackboard->SetValueAsBool("HolaBuenas", hasCheckLastPlayerPosition);

		return;
	}


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
	myBlackboard->SetValueAsBool("HolaBuenas", hasCheckLastPlayerPosition);

	myBlackboard->SetValueAsInt("WayPointIndex", idIndex + 1);
}





void ABaseEnemyController::ChecknearbyEnemy()
{
	FVector start = GetPawn()->GetActorLocation();
	FVector end = start + FVector(0, 0, 15.f);
	//a que objetos les hace caso
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetPawn());

	TArray<FHitResult> outHits;

	bool someActorClose = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(), start, end, 1500,objectTypes , false, actorsToIgnore,EDrawDebugTrace::ForDuration, outHits,true);

	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	if (someActorClose)
	{

		ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		for (FHitResult current: outHits)
		{
			if (current.GetActor() == player)
			{
				myBlackboard->SetValueAsObject("TargetActorToFollow", player);
				break;
			}
		}
	}
	else
	{
		myBlackboard->SetValueAsObject("TargetActorToFollow", NULL);

	}
}


void ABaseEnemyController::SaveLastPlayerPosition()
{
	hasCheckLastPlayerPosition = false;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	AActor* playerActor = Cast<AActor>(myBlackboard->GetValueAsObject("TargetActorToFollow"));
	myBlackboard->SetValueAsBool("HolaBuenas", hasCheckLastPlayerPosition);

	myBlackboard->SetValueAsVector("LastPlayerPosKnown", playerActor->GetActorLocation());
}


EPathFollowingRequestResult::Type ABaseEnemyController:: MoveToPlayer()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	AActor* playerActor = Cast<AActor>(myBlackboard->GetValueAsObject("TargetActorToFollow"));
	
	EPathFollowingRequestResult::Type moveToActorResult = MoveToActor(playerActor);

	return moveToActorResult;
}

