// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BaseEnemy.generated.h"

class USkeletalMeshComponent;
class UStaticMeshComponent;

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

	//Components
private:

	//USceneComponent* rootComponent{ nullptr };
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* skeletalMesh {nullptr};

	//varibles
private:
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"),Category =Movement)
	float speed{ 10 };

public:
	ACharacter* player{ nullptr };



};
