// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyManager.h"
//project
#include "BaseEnemyController.h"
#include "BaseEnemy.h"

// Sets default values
AEnemyManager::AEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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

}

void AEnemyManager::OnEnemySeePlayer()
{

}

void AEnemyManager::SpawnEnemy()
{
	//ABaseEnemy* newEnemy = GetWorld()->SpawnActor<ABaseEnemy>();
	//newEnemy->SetActorLocation(startPos);
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;

	ABaseEnemy* newEnemy = GetWorld()->SpawnActor<ABaseEnemy>(EnemyTyes[0], startPos, FRotator::ZeroRotator, SpawnParams);

	if (newEnemy)
	{
		ABaseEnemyController* controller = GetWorld()->SpawnActor<ABaseEnemyController>(AIControllerEnemyClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (controller)
		{
			controller->Possess(newEnemy);
			controller->CallBeginPlay();

		}
		newEnemy->SetActorLocation(startPos);
	}
}
