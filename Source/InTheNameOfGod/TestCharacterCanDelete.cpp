// Fill out your copyright notice in the Description page of Project Settings.


#include "TestCharacterCanDelete.h"

#include "LifeComponent.h"




// Sets default values
ATestCharacterCanDelete::ATestCharacterCanDelete()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	lifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("LifeComponent"));
}

// Called when the game starts or when spawned
void ATestCharacterCanDelete::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacterCanDelete::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATestCharacterCanDelete::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ATestCharacterCanDelete::OnDie()
{

}

