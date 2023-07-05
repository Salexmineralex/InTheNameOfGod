// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Weapon.h"
#include "MainPlayer.generated.h"


class UFollowEnemiesPoints;

UENUM(BlueprintType)
enum class EAttackAnimationsCombo : uint8
{
	defaults UMETA(Hidden),
	StartAnim  UMETA(DisplayName = "StartAnim"),
	Combo1 UMETA(DisplayName = "Combo1"),
	Combo2 UMETA(DisplayName = "Combo2"),
    	
};

UENUM(BlueprintType)
enum class EAttackInputCombo : uint8
{
	defaults UMETA(Hidden),
	Light  UMETA(DisplayName = "Light"),
	Strong UMETA(DisplayName = "Strong"),
	
    	
};

UCLASS()
class INTHENAMEOFGOD_API AMainPlayer : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	// TObjectPtr<USkeletalMeshComponent> swordMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildActor;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> Weapon;

	UPROPERTY()
	AWeapon* actualWeapon = nullptr;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	// TObjectPtr<UCapsuleComponent> swordCollision;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* niagaraDash;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFollowEnemiesPoints* followableComponent{ nullptr };

#pragma region Animation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<EAttackAnimationsCombo, UAnimMontage*> MyAnimationPool;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* walkAnimMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* attachAnimationMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* buffSwordAnimationMontage;

	// UPROPERTY(EditAnywhere)
	// UMaterialInstance* buffSwordMaterial;
	//
	// UPROPERTY(EditAnywhere)
	// UMaterialInstance* normalSwordMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackAnimationsCombo lightCombo = EAttackAnimationsCombo::StartAnim;
	
	EAttackAnimationsCombo strongCombo = EAttackAnimationsCombo::StartAnim;

	int actualInputs = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float buffTime = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float multiplayerDamage = 0.5f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool HasWeapon = true;

	bool isBuffed = false;

	bool canDash = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool enemyLocked = false;
	
	UPROPERTY()
    AActor* enemyTarget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool isJumping = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool animationBeenPlayed = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	TArray<EAttackInputCombo> inputArray;

	FTimerHandle attachWeapon{};
	
	FTimerHandle stopJump{};

	FTimerHandle dashTimer{};

	FTimerHandle buffTimer{};

#pragma endregion Animation

#pragma region Input

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BuffAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockAction;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value) override;

	void StopMoving(const FInputActionValue& Value);
	
	void Attack(const FInputActionValue& Value);
	
	void Secondary_Attack(const FInputActionValue& Value);

		
	UFUNCTION(BlueprintCallable,Category="Animation")
	void AttachWeapon();

	UFUNCTION(BlueprintCallable,Category="Animation")
	void AttachAnimation();

	virtual void Jump() override;

	virtual void StopJumping() override;

	UFUNCTION(BlueprintCallable,Category="Input")
	void Dash();

	UFUNCTION(BlueprintCallable,Category="BuffSword")
	void PlayBuffAnim();
	
	UFUNCTION(BlueprintCallable,Category="BuffSword")
	void BuffSword();

	UFUNCTION(BlueprintCallable,Category="Lock")
	void LockEnemy();

#pragma endregion Input
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(Category="Damage")
	void DamageEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION( BlueprintNativeEvent,BlueprintCallable , Category = "Animation" )
	void StartCombo(const TArray<EAttackInputCombo> &inputsArray);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable,meta=(ExpandEnumAsExecs="input"))
	void SelectAnimationByInput(TArray<EAttackInputCombo> inputs,UAnimMontage* montage,EAttackInputCombo& input,TArray<EAttackInputCombo>& outputInput);


	FTimerHandle hitStop;

	UFUNCTION(Category="Damage")
	void StartHitStop();

	UFUNCTION(Category="Damage")
	void EndHitStop();

#pragma region GetAndSet
	[[nodiscard]] bool CanAttack() const
	{
		return canAttack;
	}

	void SetCanAttack(bool bCanAttack)
	{
		canAttack = bCanAttack;
	}

	[[nodiscard]] bool IsJumping() const
	{
		return isJumping;
	}

	void SetIsJumping(bool bIsJumping)
	{
		isJumping = bIsJumping;
	}

	[[nodiscard]] bool AnimationBeenPlayed() const
	{
		return animationBeenPlayed;
	}

	void SetAnimationBeenPlayed(bool bAnimationBeenPlayed)
	{
		animationBeenPlayed = bAnimationBeenPlayed;
	}

	[[nodiscard]] bool HasWeapon1() const
	{
		return HasWeapon;
	}

	void SetHasWeapon(bool bHasWeapon)
	{
		HasWeapon = bHasWeapon;
	}

	[[nodiscard]] bool IsBuffed() const
	{
		return isBuffed;
	}

	void SetIsBuffed(bool bIsBuffed)
	{
		isBuffed = bIsBuffed;
	}

	[[nodiscard]] bool CanDash() const
	{
		return canDash;
	}

	void SetCanDash(bool bCanDash)
	{
		canDash = bCanDash;
	}
#pragma endregion GetAndSet
	
};
