// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LifeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class INTHENAMEOFGOD_API ULifeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULifeComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(BlueprintCallable)
	void GetDamage(float _damage);
	UFUNCTION(BlueprintCallable)
	void GetHeal(float _heal);
	void OnDie();
	float GetLifePercent();

	UPROPERTY(EditAnywhere,Category = Life,meta=(AllowPrivateAccess ="true"))
	float maxLife{ 100 };
	UPROPERTY(EditAnywhere,Category = Life,meta=(AllowPrivateAccess ="true"))
	float currentLife{ 100 };

private:
	

		
};
