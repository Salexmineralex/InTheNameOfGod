// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP_ThirdPerson/TP_ThirdPersonCharacter.h"
#include "MainPlayer.generated.h"

UCLASS()
class INTHENAMEOFGOD_API AMainPlayer : public ATP_ThirdPersonCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainPlayer();

	UPROPERTY(EditAnywhere)
	UAnimMontage* walkAnimMontage;

	
	UPROPERTY(EditAnywhere)
	UAnimMontage* Attack1AnimMontage;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> swordMesh;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PrimaryAction;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value) override;

	void StopMoving(const FInputActionValue& Value);
	
	void Attack(const FInputActionValue& Value);


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
