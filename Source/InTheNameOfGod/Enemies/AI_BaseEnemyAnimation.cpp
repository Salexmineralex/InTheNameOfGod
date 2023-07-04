// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_BaseEnemyAnimation.h"
//project
#include "BaseEnemy.h"
#include "BaseEnemyController.h"

void UAI_BaseEnemyAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	enemyPawn = Cast<ABaseEnemy>(TryGetPawnOwner());
}
void UAI_BaseEnemyAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!enemyPawn)
		return;

	speed = enemyPawn->GetVelocity().Size();
}
void UAI_BaseEnemyAnimation::SpawnSwordSound()
{
	if (ABaseEnemyController* control = Cast<ABaseEnemyController>(enemyPawn->GetController()))
	{
		control->SpawnSwordSound();
	}
}
void UAI_BaseEnemyAnimation::SpawnDamageSound()
{
	if (ABaseEnemyController* control = Cast<ABaseEnemyController>(enemyPawn->GetController()))
	{
		control->SpawnDamageSound();
	}
}
void UAI_BaseEnemyAnimation::SpawnDieSound()
{
	if (ABaseEnemyController* control = Cast<ABaseEnemyController>(enemyPawn->GetController()))
	{
		control->SpawnDamageSound();
	}
}