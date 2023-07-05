// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/CapsuleComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Weapon.generated.h"

UCLASS()
class INTHENAMEOFGOD_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();
	
	UPROPERTY(EditAnywhere)
	UMaterialInstance* buffSwordMaterial;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* normalSwordMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UNiagaraComponent* niagaraBuffed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float damage = 10.0f;

	bool isBuffed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<USkeletalMeshComponent> swordMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	TObjectPtr<UCapsuleComponent> swordCollision;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	[[nodiscard]] UMaterialInstance* BuffSwordMaterial() const
	{
		return buffSwordMaterial;
	}

	void SetBuffSwordMaterial(UMaterialInstance* BuffSwordMaterial)
	{
		buffSwordMaterial = BuffSwordMaterial;
	}

	[[nodiscard]] UMaterialInstance* NormalSwordMaterial() const
	{
		return normalSwordMaterial;
	}

	void SetNormalSwordMaterial(UMaterialInstance* NormalSwordMaterial)
	{
		normalSwordMaterial = NormalSwordMaterial;
	}

	[[nodiscard]] float Damage() const
	{
		return damage;
	}

	void SetDamage(float Damage)
	{
		damage = Damage;
	}

	[[nodiscard]] bool IsBuffed() const
	{
		return isBuffed;
	}

	void SetIsBuffed(bool bIsBuffed)
	{
		isBuffed = bIsBuffed;
	}

	[[nodiscard]] TObjectPtr<USkeletalMeshComponent> SwordMesh() const
	{
		return swordMesh;
	}

	void SetSwordMesh(const TObjectPtr<USkeletalMeshComponent>& SwordMesh)
	{
		swordMesh = SwordMesh;
	}

	[[nodiscard]] TObjectPtr<UCapsuleComponent> SwordCollision() const
	{
		return swordCollision;
	}

	void SetSwordCollision(const TObjectPtr<UCapsuleComponent>& SwordCollision)
	{
		swordCollision = SwordCollision;
	}
};
