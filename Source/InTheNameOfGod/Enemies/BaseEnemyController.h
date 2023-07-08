// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyManager.h"
#include "BaseEnemyController.generated.h"


class ACharacter;
class UBlackboard;
class UStaticMeshComponent;
class USoundCue;
class UFollowEnemiesPoints;
class UAnimMontage;
class UAI_BaseEnemyAnimation;
class UBehaviorTreeComponent;
class AWayPoint;

UCLASS()
class INTHENAMEOFGOD_API ABaseEnemyController : public AAIController
{
	GENERATED_BODY()
		//virtual void BeginPlay()override ;// se hace el begin play con blueprints

private:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBehaviorTree* currentTree{ nullptr };

	ACharacter* target{ nullptr };

	int wayPointsAmount{ 0 };
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* visionTrigger{ nullptr };


	UPROPERTY(VisibleAnywhere)
	FVector currentPointAroundPlayer;

	UPROPERTY(EditAnywhere)
		float ameleDistance{ 100 };
	UPROPERTY(EditAnywhere)
		float distanceToAsignPoint{ 450 };
	UPROPERTY(EditAnywhere)
		float runningSpeed{ 550 };
	UPROPERTY(EditAnywhere)
		float walkingSpeed{ 275 };
	UPROPERTY(EditAnywhere)
		float combatSpeed{ 200 };
	UPROPERTY(EditAnywhere)
		float combatDistance{ 80 };
	UPROPERTY(EditAnywhere)
		float coverProbability{ 50 };


	UFollowEnemiesPoints* followableComponent{ nullptr };

	UPROPERTY(EditInstanceOnly)
	AEnemyManager* enemyManager{nullptr};

	bool haveCalledBeginPlay{ false };





	//animations
	UAI_BaseEnemyAnimation* abpEnemy{ nullptr };
	UPROPERTY(EditDefaultsOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AM_Attack{ nullptr };
	UPROPERTY(EditDefaultsOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AM_BeHit{ nullptr };
	UPROPERTY(EditDefaultsOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	TArray<UAnimMontage*> AM_Die{ nullptr };


	UPROPERTY(EditDefaultsOnly, Category = Animations, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* AM_Cover{ nullptr };
	//sounds
	UPROPERTY(EditDefaultsOnly,Category = Sounds ,meta = (AllowPrivateAccess = "true"))
	USoundCue* attackSound01{ nullptr };
	UPROPERTY(EditDefaultsOnly,Category = Sounds ,meta = (AllowPrivateAccess = "true"))
	USoundCue* painSound{ nullptr };
	UPROPERTY(EditDefaultsOnly,Category = Sounds ,meta = (AllowPrivateAccess = "true"))
	USoundCue* dieSound{ nullptr };
	//combo attack
	int currenIndextAttack{ 0 };

	

public:
	//Getters
	bool HaveCalledBeginPlay()const { return haveCalledBeginPlay; }

	//functions
	void SetCurrentPoint(FVector newPos) { currentPointAroundPlayer = newPos; }
	void OnEnemyDie();
	void OnReciveAttack(float damage);
	void OnBeHit(float damage);
	void CallBeginPlay() { BeginPlay(); }
	
	
	
	//tasks
	UFUNCTION(Category = Tasks)
	void UpdateNextTargetPoint();

	UFUNCTION(Category = Tasks)
	void ChangeState(int newState);

	UFUNCTION(BlueprintCallable, Category = Tasks)
	EPathFollowingRequestResult::Type MoveToPlayer();

	UFUNCTION(Category = Tasks)
	void AsignNextPoint();

	UFUNCTION(Category = Tasks)
	void AlertSomeEnemies();

	UFUNCTION()
	void Attack();

	UFUNCTION()
	void ComboAttack();

	UFUNCTION()
	void Cover();
	void Uncover();
	void RecoverAfterHit();
	void DeleteEnemy();


	

	void CalculateRandomPercent();



	//Services
	UFUNCTION(Category = Services)
	void ChecknearbyEnemy();
	UFUNCTION(Category = Services)
	bool CheckCanSeePlayer();
	UFUNCTION(Category = Services)
	void SaveLastPlayerPosition();
	UFUNCTION(Category = Services)
	void UpdatePositionAroundPlayer();
	UFUNCTION(Category = Services)
	void CheckPlayerAmele();
	UFUNCTION(Category = Services)
	void KeepingDistance();
	void CheckCombatDistance();
	void CheckDistanceToAsignPoint();

	void CheckEnemyCanCombat();

	//protected:
	UFUNCTION(BlueprintCallable)
	void GetAllWayPoints();

	UFUNCTION(BlueprintCallable)
	void CPPBeginPlay();
	UFUNCTION(BlueprintCallable)
	void CPPBeginPlayPostBT();

	//variables funcionamiento
	bool hasCheckLastPlayerPosition{ true };

	int EnemyState{ 0 };//0:Patrol / 1: FollowPlayer / 2:GoingToFight / 3: WaitingForFight / 4: fighting

	UPROPERTY(EditAnywhere, Category=Buscame)
	bool hasAsignedPoint{ false };
	
	//ABP notifies
	void SpawnSwordSound();
	void SpawnDamageSound();
	void SpawnDieSound();
	




};
