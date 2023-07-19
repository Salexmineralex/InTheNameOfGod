// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Weapon.h"
#include "Components/ProgressBar.h"
#include "UI/UIW_PlayerHUD.h"
#include "MainPlayer.generated.h"


class UFollowEnemiesPoints;
class UUIW_PlayerHUD;
class UUW_MenuWidget;


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UChildActorComponent* ChildActor = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> Weapon= nullptr;

	UPROPERTY()
	AWeapon* actualWeapon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraSystem* niagaraDash = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UFollowEnemiesPoints* followableComponent{ nullptr };

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* enemyPointsParent{nullptr};
	USceneComponent* GetPointsParent() { return enemyPointsParent; }

	UPROPERTY(EditAnywhere)
	class ULifeComponent* lifeComponent{nullptr};

	//UI
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UUIW_PlayerHUD> playerWidgetType;
	UPROPERTY(BlueprintReadWrite)
	UUIW_PlayerHUD* playerWidget{ nullptr };

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUW_MenuWidget> mainMenuWidget;
	UPROPERTY() UUW_MenuWidget* mainMenuWidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UCameraComponent* firstPersonCamera = nullptr;

	UPROPERTY(EditDefaultsOnly)
		FName levelToLoad;


#pragma region Animation


	UPROPERTY(EditAnywhere)
	UAnimMontage* attachAnimationMontage= nullptr;

	UPROPERTY(EditAnywhere)
	UAnimMontage* buffSwordAnimationMontage= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackAnimationsCombo lightCombo = EAttackAnimationsCombo::StartAnim;
	
	EAttackAnimationsCombo strongCombo = EAttackAnimationsCombo::StartAnim;

	int actualInputs = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float buffTime = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float actualMana = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float maxMana = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float multiplayerDamage = 0.5f;
	
	UPROPERTY( BlueprintReadWrite)
    bool HasWeapon = true;

	bool isBuffed = false;

	bool canDash = true;

	UPROPERTY( BlueprintReadWrite)
	bool canAttack = true;

	UPROPERTY( BlueprintReadWrite)
	bool enemyLocked = false;
	
	UPROPERTY()
    AActor* enemyTarget = nullptr;

	UPROPERTY( BlueprintReadWrite)
	bool isJumping = false;

	UPROPERTY( BlueprintReadWrite)
	bool animationBeenPlayed = false;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	TArray<EAttackInputCombo> inputArray;

	FTimerHandle attachWeapon{};
	
	FTimerHandle stopJump{};

	FTimerHandle dashTimer{};

	FTimerHandle buffTimer{};

	FTimerHandle swordCollision{};
	
	FTimerHandle hitStop{};


#pragma endregion Animation

#pragma region Input

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAction=nullptr;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryAction=nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* DashAction=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BuffAction=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LockAction=nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputComponent* MyPlayerInputComponent=nullptr;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value) override;

	void StopMoving(const FInputActionValue& Value);
	
	void Attack(const FInputActionValue& Value);
	
	void Secondary_Attack(const FInputActionValue& Value);
	
	void Recover_Sword();
	
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



	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION( BlueprintNativeEvent,BlueprintCallable , Category = "Animation" )
	void StartCombo(const TArray<EAttackInputCombo> &inputsArray);
	
	UFUNCTION(BlueprintCallable,meta=(ExpandEnumAsExecs="input"))
	void SelectAnimationByInput(TArray<EAttackInputCombo> inputs,UAnimMontage* montage,EAttackInputCombo& input,TArray<EAttackInputCombo>& outputInput);


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

	UFUNCTION(BlueprintCallable)
	void GetDamage(float damage);

	UFUNCTION(BlueprintCallable)
	void GetHeal(float heal);

	[[nodiscard]] float ActualMana() const
	{
		return actualMana;
	}

	UFUNCTION(BlueprintCallable)
	void AddMana(float addedMana)
	{
		actualMana += addedMana;
		if (actualMana > maxMana)
		{
			actualMana = maxMana;
		}
		

		playerWidget->ManaBar()->SetPercent(actualMana/maxMana);
	}
	
	void SubstractMana(float substractedMana)
	{
		actualMana -= substractedMana;
	
		if(actualMana <= 0)
		{
			actualMana = 0;
		}
		

		playerWidget->ManaBar()->SetPercent(actualMana/maxMana);
	}

#pragma endregion GetAndSet
	
};
