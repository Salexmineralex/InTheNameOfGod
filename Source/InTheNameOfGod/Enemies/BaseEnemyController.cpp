// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemyController.h"
//unreal
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Kismet/GameplayStatics.h"
#include "BrainComponent.h"
#include "EngineUtils.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Sound/SoundCue.h"



//project
#include "InTheNameOfGod/AI/WayPoint.h"
#include "BaseEnemy.h"
#include "FollowEnemiesPoints.h"
#include "InTheNameOfGod/MainPlayer.h"
#include "InTheNameOfGod/Enemies/AI_BaseEnemyAnimation.h"
#include "InTheNameOfGod/LifeComponent.h"


void ABaseEnemyController::GetAllWayPoints()
{
	TArray<AActor*> allWayPoints;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWayPoint::StaticClass(), allWayPoints);
	wayPointsAmount = allWayPoints.Num();

}


void ABaseEnemyController::CPPBeginPlay()
{
	
	haveCalledBeginPlay = true;
	if (ABaseEnemy* owner = Cast<ABaseEnemy>(GetPawn()))
	{
		visionTrigger = owner->visionTrigger;
		myWayPoints = owner->patrolWayPoints;
	}
	//GetAllWayPoints();
	

	UAnimInstance* tempAbp = Cast<ABaseEnemy>(GetPawn())->GetSKMesh()->GetAnimInstance();
	abpEnemy = Cast<UAI_BaseEnemyAnimation>(tempAbp);

	
	RunBehaviorTree(currentTree);

	
}
void ABaseEnemyController::CPPBeginPlayPostBT()
{

	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	target = player;
	followableComponent = Cast<AMainPlayer>(target)->followableComponent;
	
	myBlackboard->SetValueAsObject("TargetActorToFollow", player);
	myBlackboard->SetValueAsBool("IsAbleToRunBehaviorTree", true);
	ChangeState(0);
	
}


void ABaseEnemyController::CalculateRandomPercent()
{
	int randomNum = FMath::RandRange(0, 100);
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsInt("RandomPercentForCombo", randomNum);
}



void ABaseEnemyController::UpdateNextTargetPoint()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();

	if (!hasCheckLastPlayerPosition)
	{
		hasCheckLastPlayerPosition = true;
		FVector newPos = myBlackboard->GetValueAsVector("LastPlayerPosKnown");
		myBlackboard->SetValueAsVector("WayPointPosition",newPos);
	
		return;
	}
	
	int32 idIndex = myBlackboard->GetValueAsInt("WayPointIndex");
	//if (idIndex >= Cast<ABaseEnemy>(GetPawn())->patrolWayPoints.Num())
		if (idIndex >= myWayPoints.Num())
	{
		idIndex = 0;
		myBlackboard->SetValueAsInt("WayPointIndex", idIndex);
	}
	
	//for (TActorIterator<AWayPoint> It(GetWorld()); It; ++It)
	//{
	//	AWayPoint* currentWayPoint = *It;
	//	if (currentWayPoint->idPosition == idIndex)
	//	{
	//		myBlackboard->SetValueAsVector("WayPointPosition", currentWayPoint->GetActorLocation());
	//		break;
	//	}
	//}
	//FVector newPos = Cast<ABaseEnemy>(GetPawn())->patrolWayPoints[idIndex]->GetActorLocation();
	FVector newPos = myWayPoints[idIndex]->GetActorLocation();
	myBlackboard->SetValueAsVector("WayPointPosition", newPos);

	myBlackboard->SetValueAsInt("WayPointIndex", idIndex + 1);
}





void ABaseEnemyController::ChecknearbyEnemy()
{
	if(!visionTrigger)
		return ;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	TArray<AActor*> overlapingActors;
	visionTrigger->GetOverlappingActors(overlapingActors, ACharacter::StaticClass());
			DrawDebugLine(GetWorld(), GetPawn()->GetActorLocation(), target->GetActorLocation(), FColor::Red, false,0.2);
	for (AActor* currentActor : overlapingActors)
	{
		if (currentActor == target)
		{
			FVector startPos = GetPawn()->GetActorLocation();
			FVector direction = target->GetActorLocation() - GetPawn()->GetActorLocation();
			FHitResult* hitActor = new FHitResult;
			FCollisionQueryParams CQP;
			bool hiting = GetWorld()->LineTraceSingleByChannel(*hitActor, startPos, target->GetActorLocation(), ECC_Visibility, CQP);
			if (hiting)
			{
				DrawDebugLine(GetWorld(), GetPawn()->GetActorLocation(), target->GetActorLocation(), FColor::Blue, true);


			}
			//UKismetSystemLibrary::LineTraceSingleForObjects(GetWorld(),startPos,target->GetActorLocation(),)
			myBlackboard->SetValueAsBool("CanSeePlayer", true);
			return;
		}
	}
	myBlackboard->SetValueAsBool("CanSeePlayer", false);
	return ;
}
void ABaseEnemyController::CheckCombatDistance()
{
	float distance = FVector::Distance(target->GetActorLocation(), GetPawn()->GetActorLocation());
	bool enemyInAttackDistance = distance < combatDistance+40;
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsBool("EnemyInAttackDistance", enemyInAttackDistance);
	followableComponent->enemiesKnowPlayer.Remove(this);
	if (distance >= 700)
		ChangeState(1);
}


void ABaseEnemyController::UpdatePositionAroundPlayer()
{
		UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
		myBlackboard->SetValueAsVector("PointAroundPlayer", target->GetActorLocation() + currentPointAroundPlayer);
		Cast<ABaseEnemy>(GetPawn())->puntoDeida = target->GetActorLocation() + currentPointAroundPlayer;
		ABaseEnemy* pawn = Cast<ABaseEnemy>(GetPawn());
		UCharacterMovementComponent* movComp = pawn->GetCharacterMovement();
		movComp->MaxWalkSpeed = FVector::Distance(target->GetActorLocation() + currentPointAroundPlayer, pawn->GetActorLocation()) < 500? walkingSpeed : runningSpeed;

		//MoveToLocation(target->GetActorLocation() + currentPointAroundPlayer);
		//UKismetSystemLibrary::DrawDebugSphere(GetWorld(), target->GetActorLocation() + currentPointAroundPlayer, 50,12, FLinearColor::Blue,0.5f,1);
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
		abpEnemy->SetIsInFight(false);
	}
	else if (newState == 1 || newState == 2)
	{
		movComp->MaxWalkSpeed = runningSpeed;
		abpEnemy->SetIsInFight(false);
	}
	else if (newState == 4)
	{
		movComp->MaxWalkSpeed = combatSpeed;
		abpEnemy->SetIsInFight(true);
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
void ABaseEnemyController::CheckEnemyCanCombat()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	//myBlackboard->SetValueAsBool("CanGoCombat", true);
	bool isAmele = myBlackboard->GetValueAsBool("IsPlayerAmele");
	if (isAmele)
	{
		if (followableComponent->enemiesKnowPlayer.Num() == 1)
		{
			if (followableComponent->enemiesKnowPlayer[0] == this)
			{
				myBlackboard->SetValueAsBool("CanGoCombat", true);

			}
		}
		else if (followableComponent->enemiesKnowPlayer.Num() >= 2)
		{
			if (followableComponent->enemiesKnowPlayer[1] == this)
			{
				myBlackboard->SetValueAsBool("CanGoCombat", true);

			}
		}
	}
	else
		myBlackboard->SetValueAsBool("CanGoCombat", false);

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

}

void ABaseEnemyController::CheckDistanceToAsignPoint()
{
	bool isClose = FVector::Distance(GetPawn()->GetActorLocation(), target->GetActorLocation()) < distanceToAsignPoint;

	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsBool("DistanceToAsignPoint", isClose);
	//if(!isClose)
	//ChangeState(2);
}

void ABaseEnemyController::KeepingDistance()
{
	FVector direction = target->GetActorLocation() - GetPawn()->GetActorLocation();
	direction.Z = 0;

	float distance = direction.Size();

	FVector finalPos = GetPawn()->GetActorLocation() + direction.GetSafeNormal() * (distance - combatDistance+10);

	MoveToLocation(finalPos);
	SetFocus(target);
	
}


EPathFollowingRequestResult::Type ABaseEnemyController:: MoveToPlayer()
{
	//if (followableComponent->isFightStarted)
	//	ChangeState(2);
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	AActor* playerActor = Cast<AActor>(myBlackboard->GetValueAsObject("TargetActorToFollow"));
	
	EPathFollowingRequestResult::Type moveToActorResult = MoveToActor(playerActor);

	return moveToActorResult;
}

void ABaseEnemyController::AsignNextPoint()
{
	followableComponent->AsignNewPoint(this);
	FVector hola = currentPointAroundPlayer;
	UpdatePositionAroundPlayer();
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	FVector adios = myBlackboard->GetValueAsVector("PointAroundPlayer");
	float a = 3;
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
				//control->ChangeState(2);
				if(!control->GetBrainComponent()->GetBlackboardComponent()->GetValueAsBool("CanSeePlayer"))
					control->GetBrainComponent()->GetBlackboardComponent()->SetValueAsBool("CanSeePlayer", true);
			}
		}
	}

}

void ABaseEnemyController::OnEnemyDie()
{
	followableComponent->OnEnemyDie(this);

	abpEnemy->KillEnemy();

	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsBool("IsAbleToRunBehaviorTree", false);
	enemyManager->OnEnemyDie();
}
void ABaseEnemyController::OnReciveAttack(float damage)
{
		UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
		myBlackboard->SetValueAsBool("CanGoCombat", true);

	int randomProbability = FMath::RandRange(0, 100);
	if (randomProbability <= coverProbability)
	{
		myBlackboard->SetValueAsBool("IsUnderAttack", true);
	}
	else
		OnBeHit(damage);
}
void ABaseEnemyController::OnBeHit(float damage )//FALTA PASARLE EL FLOAT CON LA CANTIDAD DE DAÑO, PERO PARA PRUEBAS NO LO PONGO
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsBool("BeHit", true);
	int randomIndex = FMath::RandRange(0, AM_BeHit.Num() - 1);
	if (AM_BeHit[randomIndex] && abpEnemy)
	{
		abpEnemy->Montage_Play(AM_BeHit[randomIndex]);
		myBlackboard->SetValueAsFloat("AttackTime", AM_BeHit[randomIndex]->GetPlayLength());
	}
	ABaseEnemy* owner = Cast<ABaseEnemy>(GetPawn());
	owner->lifeComponent->GetDamage(damage);//HABRA QUE PONER EL FLOAT QUE SE LE PASE
}

void ABaseEnemyController::RecoverAfterHit()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsBool("BeHit", false);
}

void ABaseEnemyController::Attack()
{
	int randomIndex = FMath::RandRange(0, AM_Attack.Num() - 1);
	UAnimMontage* tempAttack = AM_Attack[randomIndex];
	if (abpEnemy && tempAttack)
	{
		abpEnemy->Montage_Play(tempAttack);
	}
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsFloat("AttackTime", tempAttack->GetPlayLength());

}

void ABaseEnemyController::ComboAttack()
{
	UAnimMontage* tempAttack = AM_Attack[currenIndextAttack];
	currenIndextAttack++;
	if (currenIndextAttack >= AM_Attack.Num())
	{
		currenIndextAttack = 0;
	}

	if (abpEnemy && tempAttack)
	{
		abpEnemy->Montage_Play(tempAttack);
	}

	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsFloat("AttackTime", tempAttack->GetPlayLength()-0.75f);
}
void ABaseEnemyController::Cover()
{
	if (abpEnemy && AM_Cover)
	{
		abpEnemy->Montage_Play(AM_Cover);
	}
}
void ABaseEnemyController::Uncover()
{
	UBlackboardComponent* myBlackboard = BrainComponent->GetBlackboardComponent();
	myBlackboard->SetValueAsBool("IsUnderAttack", false);
}
void ABaseEnemyController::SpawnSwordSound()
{
	if (attackSound01)
	{
		UGameplayStatics::PlaySoundAtLocation(GetPawn(), attackSound01, GetPawn()->GetActorLocation());
	}
}
void ABaseEnemyController::SpawnDamageSound()
{
	if (attackSound01)
	{
		UGameplayStatics::PlaySoundAtLocation(GetPawn(), painSound, GetPawn()->GetActorLocation());
	}
}
void ABaseEnemyController::SpawnDieSound()
{
	if (attackSound01)
	{
		UGameplayStatics::PlaySoundAtLocation(GetPawn(), dieSound, GetPawn()->GetActorLocation());
	}
}

void ABaseEnemyController::DeleteEnemy()
{
	GetWorld()->DestroyActor(GetPawn());
}



