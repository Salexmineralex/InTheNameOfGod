// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyController.h"
//unreal
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"


//project
#include "InTheNameOfGod/AI/WayPoint.h"
#include "BaseEnemy.h"
#include "FollowEnemiesPoints.h"
#include "InTheNameOfGod/MainPlayer.h"



void ABaseEnemyController::GetAllWayPoints()
{
	TArray<AActor*> allWayPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), allWayPoints);
	wayPointsAmount = allWayPoints.Num();

}


void ABaseEnemyController::CPPBeginPlay()
{
	if (ABaseEnemy* owner = Cast<ABaseEnemy>(GetPawn()))
	{
		visionTrigger = owner->visionTrigger;
	}
	GetAllWayPoints();

}
void ABaseEnemyController::CPPBeginPlayPostBT()
{
	ChangeState(2);


	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	target = player;
	bool buenas = false;
	buenas = Cast<UFollowEnemiesPoints>(player) ? true : false;

	myBlackboard->SetValueAsObject("TargetActorToFollow", player);
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

	myBlackboard->SetValueAsInt("WayPointIndex", idIndex + 1);
}





void ABaseEnemyController::ChecknearbyEnemy()
{
	/*FVector start = GetPawn()->GetActorLocation();
	FVector end = start + FVector(0, 0, 15.f);
	//a que objetos les hace caso
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetPawn());

	TArray<FHitResult> outHits;

	bool someActorClose = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(), start, end, 30,objectTypes , false, actorsToIgnore,EDrawDebugTrace::ForDuration, outHits,true);

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

	}*/
	if(!visionTrigger)
		return ;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	TArray<AActor*> overlapingActors;
	visionTrigger->GetOverlappingActors(overlapingActors, ACharacter::StaticClass());
	for (AActor* currentActor : overlapingActors)
	{
		if (currentActor == target)
		{
			myBlackboard->SetValueAsBool("CanSeePlayer", true);
			return ;
		}
	}
			myBlackboard->SetValueAsBool("CanSeePlayer", false);
	return ;
}


void ABaseEnemyController::UpdatePositionAroundPlayer()
{
	if (currentPointAroundPlayer)
	{
		UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
		myBlackboard->SetValueAsVector("PointAroundPlayer", currentPointAroundPlayer->GetComponentLocation());
	}
}

void ABaseEnemyController::ChangeState(int newState)
{
	EnemyState = newState;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsInt("EnemyState", EnemyState);
}

bool ABaseEnemyController::CheckCanSeePlayer()
{
	if (!visionTrigger)
		return false;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();

	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	TArray<AActor*> overlapingActors;
	visionTrigger->GetOverlappingActors(overlapingActors, ACharacter::StaticClass());
	for (AActor* currentActor : overlapingActors)
	{
		if (currentActor == player)
		{
			myBlackboard->SetValueAsObject("TargetActorToFollow", currentActor);
			return true;
		}
	}
	myBlackboard->SetValueAsObject("TargetActorToFollow", NULL);
	return false;


}


void ABaseEnemyController::SaveLastPlayerPosition()
{
	hasCheckLastPlayerPosition = false;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	AActor* playerActor = Cast<AActor>(myBlackboard->GetValueAsObject("TargetActorToFollow"));

	myBlackboard->SetValueAsVector("LastPlayerPosKnown", playerActor->GetActorLocation());
}


EPathFollowingRequestResult::Type ABaseEnemyController:: MoveToPlayer()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	AActor* playerActor = Cast<AActor>(myBlackboard->GetValueAsObject("TargetActorToFollow"));
	
	EPathFollowingRequestResult::Type moveToActorResult = MoveToActor(playerActor);

	return moveToActorResult;
}

void ABaseEnemyController::AsignNextPoint()
{
	if (hasAsignedPoint)
		return;
	if (AMainPlayer* player = Cast<AMainPlayer>(target))
	{
		if (player->followableComponent)
		{
			currentPointAroundPlayer = player->followableComponent->AsignNewPoint();
			UpdatePositionAroundPlayer();
		}
		hasAsignedPoint = true;
	}
}


