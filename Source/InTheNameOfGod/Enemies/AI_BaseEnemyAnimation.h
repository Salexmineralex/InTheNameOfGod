// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AI_BaseEnemyAnimation.generated.h"

/**
 * 
 */
class ABaseEnemy;
class AbaseEnemyController;
UCLASS()
class INTHENAMEOFGOD_API UAI_BaseEnemyAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void NativeUpdateAnimation(float DeltaSeconds);
	virtual void NativeInitializeAnimation()override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	ABaseEnemy* enemyPawn{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float speed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool isInFight{ false };
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool isDie{ false };
public:
	void SetIsInFight(bool value) { isInFight = value; }
	UFUNCTION(BlueprintCallable)
	void SpawnSwordSound();
	UFUNCTION(BlueprintCallable)
	void SpawnDamageSound();
	UFUNCTION(BlueprintCallable)
	void SpawnDieSound();
	void KillEnemy() { isDie = true; }

};
