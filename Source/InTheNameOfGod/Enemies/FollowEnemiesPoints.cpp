// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowEnemiesPoints.h"
//unreal
#include "Kismet/KismetSystemLibrary.h"
//project
#include "InTheNameOfGod/MainPlayer.h"
#include "InTheNameOfGod/Enemies/BaseEnemyController.h"
#include "InTheNameOfGod/Enemies/BaseEnemy.h"

// Sets default values for this component's properties
UFollowEnemiesPoints::UFollowEnemiesPoints()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	// ...
}


// Called when the game starts
void UFollowEnemiesPoints::BeginPlay()
{
	Super::BeginPlay();

	//if (AMainPlayer* player = Cast<AMainPlayer>(GetOwner()))
	//{
	//	parentPoints = player->GetPointsParent();
	//}
	//CheckCloseEnemies();

	for (int i = 0; i < 5; i++)
	{
		FPointsRange newRange;
		newRange.radiusRange = 300 + i * 150;
		newRange.pointsAmount = 4 + i * 3;
		posibleEnemypoints.Add(newRange);
	}

}
	// ...

	




// Called every frame
void UFollowEnemiesPoints::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UFollowEnemiesPoints::RecolocatePoints()
{

	for (FPointsRange group : posibleEnemypoints)
	{
		float radius = group.radiusRange;
		float angleDiference = 360 / group.points.Num();
		float currentAngle = 0;

		FVector origin = GetOwner()->GetActorLocation();
	}
}

void UFollowEnemiesPoints::AsignNewPoint(ABaseEnemyController* enemy)
{
	enemiesKnowPlayer.Add(enemy);
	int totalEnemies = enemiesKnowPlayer.Num()-2;

	if(totalEnemies>2)
	for (FPointsRange& range : posibleEnemypoints)
	{
		int pointsToPrepare = totalEnemies >= range.pointsAmount ? range.pointsAmount : totalEnemies;

		float angleDiference = 360 / pointsToPrepare;
		float currentAngle = 0;
		range.points.Empty();
		for (int i = 0; i < pointsToPrepare; i++)
		{
			float radians = FMath::DegreesToRadians(currentAngle);
			FVector finalPos = FVector(FMath::Cos(radians), FMath::Sin(radians), 0.f) * range.radiusRange;
			range.points.Add(finalPos);
			currentAngle += angleDiference;
			DrawDebugSphere(GetWorld(), (GetOwner()->GetActorLocation() + finalPos), 10, 5, FColor::Red ,true,300,1,1);
		}
		if (totalEnemies <= range.pointsAmount)
			break;
		else
			totalEnemies -= range.pointsAmount;

	}

	if(enemiesKnowPlayer.Num()>1)
	for (int i = 0; i < enemiesKnowPlayer.Num() - 1; i++)
	{
		for (int j = 0; j < enemiesKnowPlayer.Num() - i - 1; j++)
		{
			float distanceA = FVector::Distance(GetOwner()->GetActorLocation(), enemiesKnowPlayer[j]->GetPawn()->GetActorLocation());
			float distanceB = FVector::Distance(GetOwner()->GetActorLocation(), enemiesKnowPlayer[j + 1]->GetPawn()->GetActorLocation());
			if (distanceA > distanceB)
			{
				ABaseEnemyController* temp = enemiesKnowPlayer[j];
				enemiesKnowPlayer[j] = enemiesKnowPlayer[j + 1];
				enemiesKnowPlayer[j + 1] = temp;
			}
		}
	}
	int enemyIndex = enemiesKnowPlayer.Num() - 1;
	enemiesKnowPlayer[0]->SetCurrentPoint(FVector::ZeroVector);
	if (enemiesKnowPlayer.Num() > 1)
	enemiesKnowPlayer[1]->SetCurrentPoint(FVector::ZeroVector);

	if (totalEnemies > 2)
	{
		int currentRangePos = 0;
		int currentRange = 0;
		for (int i = 2; i < enemiesKnowPlayer.Num(); i++)
		{
			enemiesKnowPlayer[i]->SetCurrentPoint(posibleEnemypoints[currentRange].points[currentRangePos]);
			currentRangePos++;
			if (currentRangePos >= posibleEnemypoints[currentRange].pointsAmount)
			{
				currentRangePos = 0;
				currentRange++;
			}
		}

	}

}




void UFollowEnemiesPoints::CheckCloseEnemies()
{
	FVector initPos = GetOwner()->GetActorLocation();
	FVector endPos = initPos + FVector(0, 0, checkDistance);

	//a que objetos les hace caso
	TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));

	//ActorsToIgnore
	TArray<AActor*> actorsToIgnore;
	actorsToIgnore.Add(GetOwner());

	//actores
	TArray<FHitResult> outHits;

	bool someEnemyClose = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(), initPos, endPos, checkDistance, objectTypes, false, actorsToIgnore, EDrawDebugTrace::ForDuration, outHits, true
	);
	if (someEnemyClose)
	{
		for (int i = 0; i < outHits.Num() - 1; i++)
		{
			for (int j = 0; j < outHits.Num() - i - 1; j++)
			{
				float distanceA = FVector::Distance(GetOwner()->GetActorLocation(), outHits[j].GetActor()->GetActorLocation());
				float distanceB = FVector::Distance(GetOwner()->GetActorLocation(), outHits[j+1].GetActor()->GetActorLocation());
				if (distanceA > distanceB)
				{
					FHitResult temp = outHits[j];
					outHits[j] = outHits[j + 1];
					outHits[j + 1] = temp;
				}
			}
		}

		for (int i = 0; i < outHits.Num(); i++)
		{
			//if (ABaseEnemyController* currentEnemy = Cast<ABase>(outHits[i].GetActor()))
			if (ABaseEnemy* currentEnemy = Cast<ABaseEnemy>(outHits[i].GetActor()))
			{
				if (ABaseEnemyController* controller = Cast<ABaseEnemyController>(currentEnemy->Controller))
				{
					controller->hasAsignedPoint = false;
					controller->AsignNextPoint();
				}
			}

		}
	}

}
void UFollowEnemiesPoints::CheckCloseEnemies(TArray<FHitResult> outHits)
{
	//FVector initPos = GetOwner()->GetActorLocation();
	//FVector endPos = initPos + FVector(0, 0, checkDistance);
	//
	////a que objetos les hace caso
	//TArray<TEnumAsByte<EObjectTypeQuery>> objectTypes;
	//objectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	//
	////ActorsToIgnore
	//TArray<AActor*> actorsToIgnore;
	//actorsToIgnore.Add(GetOwner());
	//
	////actores
	//TArray<FHitResult> outHits;
	//
	//bool someEnemyClose = UKismetSystemLibrary::SphereTraceMultiForObjects(
	//	GetWorld(), initPos, endPos, checkDistance, objectTypes, false, actorsToIgnore, EDrawDebugTrace::ForDuration, outHits, true
	//);
	//if (someEnemyClose)
	//{
		for (int i = 0; i < outHits.Num() - 1; i++)
		{
			for (int j = 0; j < outHits.Num() - i - 1; j++)
			{
				float distanceA = FVector::Distance(GetOwner()->GetActorLocation(), outHits[j].GetActor()->GetActorLocation());
				float distanceB = FVector::Distance(GetOwner()->GetActorLocation(), outHits[j + 1].GetActor()->GetActorLocation());
				if (distanceA > distanceB)
				{
					FHitResult temp = outHits[j];
					outHits[j] = outHits[j + 1];
					outHits[j + 1] = temp;
				}
			}
		}

		for (int i = 0; i < outHits.Num(); i++)
		{
			//if (ABaseEnemyController* currentEnemy = Cast<ABase>(outHits[i].GetActor()))
			if (ABaseEnemy* currentEnemy = Cast<ABaseEnemy>(outHits[i].GetActor()))
			{
				if (ABaseEnemyController* controller = Cast<ABaseEnemyController>(currentEnemy->Controller))
				{
					controller->hasAsignedPoint = false;
					controller->AsignNextPoint();
				}
			}

		}
	//}

}

void UFollowEnemiesPoints::OnEnemyDie(ABaseEnemyController* enemy)
{
	enemiesKnowPlayer.Remove(enemy);
	int totalEnemies = enemiesKnowPlayer.Num();
	for (FPointsRange& range : posibleEnemypoints)
	{
		int pointsToPrepare = totalEnemies >= range.pointsAmount ? range.pointsAmount : totalEnemies;

		float angleDiference = 360 / pointsToPrepare;
		float currentAngle = 0;
		range.points.Empty();
		for (int i = 0; i < pointsToPrepare; i++)
		{
			float radians = FMath::DegreesToRadians(currentAngle);
			FVector finalPos = FVector(FMath::Cos(radians), FMath::Sin(radians), 0.f) * range.radiusRange;
			range.points.Add(finalPos);
			currentAngle += angleDiference;
			DrawDebugSphere(GetWorld(), (GetOwner()->GetActorLocation() + finalPos), 10, 5, FColor::Red, true, 300, 1, 1);
		}
		if (totalEnemies <= range.pointsAmount)
			break;
		else
			totalEnemies -= range.pointsAmount;

	}

	int currentRangePos = 0;
	int currentRange = 0;
	for (int i = 0; i < enemiesKnowPlayer.Num(); i++)
	{
		enemiesKnowPlayer[i]->SetCurrentPoint(posibleEnemypoints[currentRange].points[currentRangePos]);
		currentRangePos++;
		if (currentRangePos >= posibleEnemypoints[currentRange].pointsAmount)
		{
			currentRangePos = 0;
			currentRange++;
		}
	}

}