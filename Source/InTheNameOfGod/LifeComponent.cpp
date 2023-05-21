// Fill out your copyright notice in the Description page of Project Settings.


#include "LifeComponent.h"
#include "TestCharacterCanDelete.h"
//#include "BaseEnemy.h"

// Sets default values for this component's properties
ULifeComponent::ULifeComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULifeComponent::BeginPlay()
{
	Super::BeginPlay();
	currentLife = maxLife;
	// ...
	
}


// Called every frame
void ULifeComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void ULifeComponent::GetDamage(float _damage)
{
	currentLife -= _damage;
	if (currentLife <= 0)
	{
		OnDie();
	}
}

void ULifeComponent::GetHeal(float _heal)
{
	currentLife += _heal;
	if (currentLife > maxLife)
	{
		currentLife = maxLife;
	}
}

void ULifeComponent::OnDie()
{
	//primero el del enemigo que recibira daño mas veces que el player y algo optimizará
	//if (ABaseEnemy* newChar = Cast<ABaseEnemy>(GetOwner()))//esto con todas las clases que tengan un lifeComponent.
	//{
	//	newChar->OnDie();
	//	return;
	//}
	if (ATestCharacterCanDelete* newChar = Cast<ATestCharacterCanDelete>(GetOwner()))//esto con todas las clases que tengan un lifeComponent.
	{
		newChar->OnDie();
		return;
	}
}
float ULifeComponent::GetLifePercent()
{
	return ((currentLife * 100) / maxLife);
}
