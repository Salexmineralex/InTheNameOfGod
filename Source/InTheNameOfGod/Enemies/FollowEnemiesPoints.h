// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"



#include "FollowEnemiesPoints.generated.h"

USTRUCT(Blueprintable)
struct FEnemyFinalPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	USceneComponent* pointPosition{nullptr};

	bool isFull{ false };

};

USTRUCT(Blueprintable)
struct FPointsRange
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	TArray<FEnemyFinalPoint> points;
	UPROPERTY(EditAnyWhere)
		float radiusRange{ 30 };
	UPROPERTY(EditAnyWhere)
		int pointsAmount{ 0 };
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTHENAMEOFGOD_API UFollowEnemiesPoints : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UFollowEnemiesPoints();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere)
	TArray<FPointsRange> posibleEnemypoints;

	UPROPERTY(EditAnywhere)
	USceneComponent* buenasTardes{nullptr};
		
};
