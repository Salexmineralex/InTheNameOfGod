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

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USceneComponent* pointPosition{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isFull{ false };

	AActor* currentEnemyAtPoint{ nullptr };

	void SwitchIsFull() { isFull = !isFull; }

};

USTRUCT(Blueprintable)
struct FPointsRange
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemyFinalPoint> points;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float radiusRange{ 30 };
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category = Points)
	TArray<FPointsRange> posibleEnemypoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = Points)
	float radiusRangeOffset{ 10 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
		float firstRadius{ 10 };

	UFUNCTION(BlueprintCallable)
	void RecolocatePoints();

	USceneComponent* parentPoints{ nullptr };
	bool canRotatePoints{ true };
	int currentEnemiesGoingPlayer{0};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
		float checkDistance{ 20 };

	void CheckCloseEnemies();

	//enemy
	USceneComponent* AsignNewPoint();



		
};
