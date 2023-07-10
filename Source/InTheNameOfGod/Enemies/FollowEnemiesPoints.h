// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/SceneComponent.h"



#include "FollowEnemiesPoints.generated.h"
class ABaseEnemyController;
USTRUCT(Blueprintable)
struct FEnemyFinalPoint
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector pointPosition = FVector();

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
	TArray<FVector> points;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		float radiusRange{ 30 };
	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
		int pointsAmount{ 0 };
	UPROPERTY(VisibleAnywhere)
	int enemiesInAmount{ 0 };
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

	UPROPERTY(VisibleAnywhere)
		TArray<ABaseEnemyController*> enemiesKnowPlayer;

	//bool isFightStarted{ false };

	//enemy
	void CheckCloseEnemies();
	void CheckCloseEnemies(TArray<FHitResult> outhits);
	void AsignNewPoint(ABaseEnemyController* enemy);
	void OnEnemyDie(ABaseEnemyController* enemy);



		
};
