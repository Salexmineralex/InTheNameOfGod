// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EnemyManager.generated.h"

class ABaseEnemy;
class AWayPoint;

UCLASS()
class INTHENAMEOFGOD_API AEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void OnEnemySeePlayer();

	
private:

	UPROPERTY(EditAnywhere)
	TArray<ABaseEnemy*> enemiesInScene;
	UPROPERTY(EditAnywhere, Category = Spawn ,meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ABaseEnemy>>EnemyTyes;
	UPROPERTY(EditInstanceOnly,Category = GameFlow, meta = (AllowPrivateAccess = "true"))
	TArray<AWayPoint*> wayPoints;
	UFUNCTION()
	TArray<int> GetRandomForWayPoints();

	bool isPlayerIn{ false };

	int currentEnemies{ 0 };
	int currentSpawnedEnemies{ 0 };
	UPROPERTY(EditInstanceOnly, Category = GameFlow, meta = (AllowPrivateAccess = "true"))
		int maxEnemies{ 10 };
	UPROPERTY(EditInstanceOnly, Category = GameFlow, meta = (AllowPrivateAccess = "true"))
		int maxEnemiesToSpawn;
	UPROPERTY(EditInstanceOnly, Category = GameFlow, meta = (AllowPrivateAccess = "true"))
		float spawnRate{ 10 };
	float currentTime{ 0 };
	UPROPERTY(EditAnywhere, Category = GameFlow, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* collisionEnter{nullptr};
public:
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseEnemyController> AIControllerEnemyClass;

	UPROPERTY(EditAnywhere)
	FVector startPos;
	UFUNCTION(CallInEditor)
	void SpawnEnemy();
	TArray<AWayPoint*> SetEnemyPatrolPoints();
	void OnEnemyDie() { currentEnemies--; }
	UFUNCTION()
	void OnPlayerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
		UFUNCTION()
	void OnPlayerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


};
