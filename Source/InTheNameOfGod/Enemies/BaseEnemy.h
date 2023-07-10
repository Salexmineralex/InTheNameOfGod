// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"

#include "BaseEnemy.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;
class UBoxComponent;
class ULifeComponent;
class AWayPoint;
class ABaseEnemyController;

UCLASS()
class INTHENAMEOFGOD_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(CallInEditor, Category = AAArlon)
	void BeUnderAttack();
	UFUNCTION(CallInEditor, Category = AAArlon)
	void BeHit();

	UPROPERTY(EditAnywhere, Category = ObjectsToSpawn)
	TSubclassOf<class APickableObject> Life_Potion;

	UPROPERTY(EditAnywhere, Category = ObjectsToSpawn)
	TSubclassOf<class APickableObject> Mana_Potion;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* swordCollision{nullptr};

	//Components
private:
	ABaseEnemyController* controller = nullptr;
	//USceneComponent* rootComponent{ nullptr };
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* skeletalMesh {nullptr};

	UPROPERTY(EditDefaultsOnly, Category = Equipment, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* swordMesh{nullptr};
	UPROPERTY(EditDefaultsOnly,Category = Equipment, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* shieldMesh{nullptr};
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		TSubclassOf<ABaseEnemyController> AIControllerEnemyClass;




public:
	UPROPERTY(EditInstanceOnly, Category = Patrol)
		TArray<AWayPoint*> patrolWayPoints;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* visionTrigger {nullptr};

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	ULifeComponent* lifeComponent {nullptr};
	
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UWidgetComponent* whiteballComponent {nullptr};

	//varibles
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"),Category =Movement)
	float speed{ 10 };
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"), Category = Movement)
	float damage{ 10 };

public:
	UPROPERTY(VisibleAnyWhere)
	FVector puntoDeida;
	ACharacter* player{ nullptr };
	UFUNCTION(CallInEditor)
	void OnDie();

	FTimerHandle swordCollisionTimerHandle{};
	void Recover_Sword();
	USkeletalMeshComponent* GetSKMesh() { return skeletalMesh; }

	void AttachEquipment();
	void SetWayPoints(TArray<AWayPoint*> points) { patrolWayPoints = points; }
	UFUNCTION()
	void DamagePlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);



};
