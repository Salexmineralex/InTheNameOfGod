// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowEnemiesPoints.h"
//#include "Math/RandomStream.h"
//#include "Math/UnrealMathUtility.h"
// Sets default values for this component's properties
//#include "InTheNameOfGod/Enemies/BaseEnemyController"

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
	
}
	// ...

	




// Called every frame
void UFollowEnemiesPoints::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

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