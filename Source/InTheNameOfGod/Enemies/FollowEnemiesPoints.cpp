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


}
	// ...

	




// Called every frame
void UFollowEnemiesPoints::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (!canRotatePoints&& parentPoints)
	{
		parentPoints->SetWorldRotation(FRotator::ZeroRotator);
	}

	// ...
}

void UFollowEnemiesPoints::RecolocatePoints()
{

	for (FPointsRange group : posibleEnemypoints)
	{
		float radius = group.radiusRange;
		float angleDiference = 360 / group.points.Num();
		float currentAngle = 0;

		FVector origin = GetOwner()->GetActorLocation();

		for (FEnemyFinalPoint point : group.points)
		{
			FRandomStream RandomStream(FMath::Rand());
			float newRadius = RandomStream.FRandRange(radius - 10, radius + 10);
			float newAngle = RandomStream.FRandRange(currentAngle - 3, currentAngle + 3);
			float radians = FMath::DegreesToRadians(newAngle);

			FVector finalPos = FVector(FMath::Cos(radians), FMath::Sin(radians), 0.f) * newRadius;
			point.pointPosition->SetRelativeLocation( finalPos);

			currentAngle += angleDiference;


		}
	}
}

USceneComponent* UFollowEnemiesPoints::AsignNewPoint()
{

	currentEnemiesGoingPlayer++;
	if (currentEnemiesGoingPlayer == 1)
		return parentPoints;
	if (currentEnemiesGoingPlayer >= 5)
		canRotatePoints = false;

	for (FPointsRange& group : posibleEnemypoints)
	{
		int max = group.points.Num();
		int currentPoint = 0;
		
		for (int i = 0; i < group.points.Num(); i++)
		{
			if (i % 2 == 0)
			{
				currentPoint = (int)i / 2;
			}
			else
			{
				currentPoint = (int)max / 2 + (int)i / 2;
			}
			if (!group.points[currentPoint].isFull)
			{
				group.points[currentPoint].SwitchIsFull();
				bool tardes = group.points[currentPoint].isFull;
				return group.points[currentPoint].pointPosition;
			}
		}
		if (max % 2 != 0)
		{
			if (!group.points[max-1].isFull)
			{
				group.points[max-1].SwitchIsFull();
				return group.points[max - 1].pointPosition;
			}
		}


	}
	return nullptr;
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