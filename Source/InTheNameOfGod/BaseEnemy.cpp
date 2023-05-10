// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
#include "LifeComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components
	root = CreateDefaultSubobject<USceneComponent>(TEXT("root"));
	SetRootComponent(root);
	body=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("body"));
	body->SetupAttachment(root);



	lifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("lifeComponent"));

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ABaseEnemy::OnDie()
{
	GEngine->AddOnScreenDebugMessage(0, 2, FColor::Red, "MeMoriperro");
}

