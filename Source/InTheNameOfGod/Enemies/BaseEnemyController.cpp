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
#include "GameFramework/CharacterMovementComponent.h"



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
	ChangeState(0);


	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	target = player;
	followableComponent = Cast<AMainPlayer>(target)->followableComponent;

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
	if(!visionTrigger)
		return ;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();

	//ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

	TArray<AActor*> overlapingActors;
	visionTrigger->GetOverlappingActors(overlapingActors, ACharacter::StaticClass());
	for (AActor* currentActor : overlapingActors)
	{
		if (currentActor == target)
		{
			//FVector myLocation = GetPawn()->GetActorLocation();
			//FVector targetLocation = target->GetActorLocation();
			//
			//FVector Direction = targetLocation - myLocation;
			//Direction.Normalize();
			//FHitResult HitResult;
			//FCollisionQueryParams Params(NAME_None, false, target);
			//Params.AddIgnoredActor(GetPawn());
			//
			//bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, myLocation, targetLocation, ECC_Visibility, Params);
			//AActor* actorColl = HitResult.GetActor();
			//bool isPlayer = HitResult.GetActor() == target;

			myBlackboard->SetValueAsBool("CanSeePlayer", true);
			return;
		}
	}
	myBlackboard->SetValueAsBool("CanSeePlayer", false);
	return ;
}


void ABaseEnemyController::UpdatePositionAroundPlayer()
{
	//if (currentPointAroundPlayer)
	{
		UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
		myBlackboard->SetValueAsVector("PointAroundPlayer", target->GetActorLocation() + currentPointAroundPlayer);
	}
}

void ABaseEnemyController::ChangeState(int newState)
{
	EnemyState = newState;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsInt("EnemyState", EnemyState);
	ABaseEnemy* pawn = Cast<ABaseEnemy>(GetPawn());
	UCharacterMovementComponent* movComp = pawn->GetCharacterMovement();
	if (newState == 0)
	{
		movComp->MaxWalkSpeed = walkingSpeed;
	}
	else if (newState == 1 || newState == 2)
	{
		movComp->MaxWalkSpeed = runningSpeed;
	}
	else if (newState == 4)
	{
		movComp->MaxWalkSpeed = combatSpeed;
	}
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
void ABaseEnemyController::CheckPlayerAmele()
{
	bool isAmele = FVector::Distance(GetPawn()->GetActorLocation(), target->GetActorLocation()) < ameleDistance;

	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsBool("IsPlayerAmele", isAmele);
	if (isAmele)
	{
		ChangeState(4);
		followableComponent->isFightStarted = true;
	}
}


EPathFollowingRequestResult::Type ABaseEnemyController:: MoveToPlayer()
{
	if (followableComponent->isFightStarted)
		ChangeState(2);
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	AActor* playerActor = Cast<AActor>(myBlackboard->GetValueAsObject("TargetActorToFollow"));
	
	EPathFollowingRequestResult::Type moveToActorResult = MoveToActor(playerActor);

	return moveToActorResult;
}

void ABaseEnemyController::AsignNextPoint()
{

	followableComponent->AsignNewPoint(this);
	UpdatePositionAroundPlayer();

}

void ABaseEnemyController::AlertSomeEnemies()
{
	FVector initPos = GetPawn()->GetActorLocation();
	FVector endPos = initPos + FVector(0, 0, 20);

	//a que objetos les hace caso
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	//ActorsToIgnore
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetPawn());

	//actores
	TArray<FHitResult> outHits;

	bool someEnemyClose = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(), initPos, endPos, 900, objectTypes, false, actorsToIgnore, EDrawDebugTrace::ForDuration, outHits, true);
	if (someEnemyClose)
	{
		for (FHitResult object : outHits)
		{
			if (ABaseEnemy* enemy = Cast<ABaseEnemy>(object.GetActor()))
			{
 				ABaseEnemyController* control = Cast<ABaseEnemyController>(enemy->GetController());
				control->ChangeState(1);
				//control->GetBrainComponent()->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", true);
			}
		}
	}

}

void ABaseEnemyController::OnEnemyDie()
{
	if (AMainPlayer* player = Cast<AMainPlayer>(target))
	{
		if (player->followableComponent)
		{
			player->followableComponent->OnEnemyDie(this);
		}
	}
	GetWorld()->DestroyActor(GetPawn());
}

void ABaseEnemyController::Attack()
{
	UAnimInstance* abpEnemy = Cast<ABaseEnemy>(GetPawn())->GetSKMesh()->GetAnimInstance();
	if (abpEnemy && AM_attack_01)
	{
		abpEnemy->Montage_Play(AM_attack_01);
	}
}



