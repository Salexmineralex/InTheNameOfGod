// Fill out your copyright notice in the Description page of Project Settings.


#include "FollowEnemiesPoints.h"

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

	// ...
	buenasTardes = NewObject<USceneComponent>(GetOwner());
	buenasTardes->SetupAttachment(GetOwner()->GetRootComponent());
		//Crear puntos 
	/*for (FPointsRange group : posibleEnemypoints)
	{
		for (int i = 0; i < group.pointsAmount; i++)
		{
			//USceneComponent* newPoint = GetOwner()->CreateDefaultSubobject<USceneComponent>(TEXT("ENEMY POINT"));
			USceneComponent* newItem = NewObject<USceneComponent>(this);
			newItem->SetupAttachment(GetOwner()->GetRootComponent());
			newItem->RegisterComponent();

			FEnemyFinalPoint hola;
			hola.pointPosition = newItem;
			group.points.Add(hola);

		}

	}*/
	
}



// Called every frame
void UFollowEnemiesPoints::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

