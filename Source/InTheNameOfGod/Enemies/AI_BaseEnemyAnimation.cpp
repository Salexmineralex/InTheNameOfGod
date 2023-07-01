// Fill out your copyright notice in the Description page of Project Settings.


#include "AI_BaseEnemyAnimation.h"
//project
#include "BaseEnemy.h"

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