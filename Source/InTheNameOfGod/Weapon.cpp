// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	swordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("swordMesh"));
	swordMesh->SetupAttachment(RootComponent);

	swordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("swordCollision"));
	swordCollision->SetupAttachment(swordMesh);

	niagaraBuffed = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	niagaraBuffed->SetupAttachment(swordMesh);

}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

