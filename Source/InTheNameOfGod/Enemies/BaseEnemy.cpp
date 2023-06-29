// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "BaseEnemyController.h"





// Sets default values
ABaseEnemy::ABaseEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Components
	//rootComponent=CreateDefaultSubobject<USceneComponent>(TEXT("rootComponent"));
	//SetRootComponent(rootComponent);
	
	skeletalMesh=CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("skeletalMesh"));
	skeletalMesh->SetupAttachment(RootComponent);

	visionTrigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("visionTrigger"));
	visionTrigger->SetupAttachment(RootComponent);
	visionTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	visionTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	visionTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
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
	if (ABaseEnemyController* controller = Cast<ABaseEnemyController>(GetController()))
	{
		controller->OnEnemyDie();
	}
}

