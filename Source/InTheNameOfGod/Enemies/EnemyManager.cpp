// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
//unreal
#include "Components/BoxComponent.h"
//project
#include "BaseEnemyController.h"
#include "BaseEnemy.h"
#include "InTheNameOfGod/AI/WayPoint.h"
#include "InTheNameOfGod/MainPlayer.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	collisionEnter = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionEnter"));
	collisionEnter->SetupAttachment(RootComponent);
	collisionEnter->SetCollisionProfileName(TEXT("OverlapAll")); // Configura el perfil de colisión para permitir el solapamiento con otros actores

	collisionEnter->OnComponentBeginOverlap.AddDynamic(this, &AEnemyManager::OnPlayerEnter);
	collisionEnter->OnComponentEndOverlap.AddDynamic(this, &AEnemyManager::OnPlayerExit);
}

// Called when the game starts or when spawned
void AEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!isPlayerIn)
		return;
	currentTime -= GetWorld()->GetDeltaSeconds();
	if (currentTime <= 0)
	{
		currentTime = spawnRate;
		if (currentEnemies < maxEnemies)
		{
			SpawnEnemy();
		}
	}

}

void AEnemyManager::OnEnemySeePlayer()
{

}

void AEnemyManager::SpawnEnemy()
{
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ABaseEnemy* newEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyTyes[0], startPos, FRotator::ZeroRotator, SpawnParams);
	if (newEnemy)
	{
		newEnemy->SetWayPoints(SetEnemyPatrolPoints());
		ABaseEnemyController* controller = GetWorld()->SpawnActor<ABaseEnemyController>(AIControllerEnemyClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (controller)
		{
			controller->Possess(newEnemy);
			controller->CallBeginPlay();
			controller->SetEnemyManager(this);
		}
		newEnemy->SetActorLocation(startPos);
		currentEnemies++;
		currentSpawnedEnemies++;
		if (currentSpawnedEnemies >= maxEnemiesToSpawn)
			SetActorTickEnabled(false);
	}
}

TArray<AWayPoint*> AEnemyManager::SetEnemyPatrolPoints()
{
	TArray<int> indexPoints = GetRandomForWayPoints();
	TArray <AWayPoint*> points;

	for (int i = 0; i < indexPoints.Num(); i++)
	{
		points.Add(wayPoints[indexPoints[i]]);
	}
	return points;
}
TArray<int> AEnemyManager::GetRandomForWayPoints()
{
	TArray<int> points;
	
	int amount = FMath::RandRange(3, 4);
	int maxRand = wayPoints.Num()-1;
	for (int i = 0; i < amount; i++)
	{
		int indexToAdd = -1;
		indexToAdd = FMath::RandRange(0, maxRand);
		if (!points.Contains(indexToAdd))
			points.Add(indexToAdd);
		else
			amount++;

	}
	return points;
}
void AEnemyManager::OnPlayerEnter(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainPlayer* player = Cast<AMainPlayer>(OtherActor))
	{
		isPlayerIn = true;
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Red, "eSTOYdENTRO");
	}
}
void AEnemyManager::OnPlayerExit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (AMainPlayer* player = Cast<AMainPlayer>(OtherActor))
	{
		isPlayerIn = false;
	GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "eSTOYdFUERA");
	}
}
