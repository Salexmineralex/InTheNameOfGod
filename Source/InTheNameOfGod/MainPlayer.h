// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "MainPlayer.generated.h"


	
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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> swordMesh;

#pragma region Animation

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TMap<EAttackAnimationsCombo, UAnimMontage*> MyAnimationPool;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* walkAnimMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAttackAnimationsCombo lightCombo = EAttackAnimationsCombo::StartAnim;
	
	EAttackAnimationsCombo strongCombo = EAttackAnimationsCombo::StartAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool canAttack = true;

	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Animation")
	TArray<EAttackInputCombo> inputArray;



#pragma endregion Animation

#pragma region Input

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* SecondaryAction;


	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value) override;

	void StopMoving(const FInputActionValue& Value);
	
	void Attack(const FInputActionValue& Value);
	
	void Secondary_Attack(const FInputActionValue& Value);

#pragma endregion Input
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	UFUNCTION( BlueprintNativeEvent,BlueprintCallable , Category = "Animation" )
	void StartCombo(const TArray<EAttackInputCombo> &inputsArray);

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable,meta=(ExpandEnumAsExecs="input"))
	void SelectAnimationByInput(TArray<EAttackInputCombo> inputs,UAnimMontage* montage,EAttackInputCombo& input,TArray<EAttackInputCombo>& outputInput);

};
