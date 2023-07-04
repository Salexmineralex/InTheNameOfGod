// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
//unreal
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
//project
#include "BaseEnemyController.h"
#include "InTheNameOfGod/LifeComponent.h"
#include "InTheNameOfGod/AI/WayPoint.h"







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

	lifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("LifeComponent"));


	visionTrigger = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("visionTrigger"));
	visionTrigger->SetupAttachment(RootComponent);
	visionTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	visionTrigger->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	visionTrigger->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	swordMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SwordMesh"));
	shieldMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShieldMesh"));
}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	AttachEquipment();

}
void ABaseEnemy::BeUnderAttack()
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(GetController());
	control->OnReciveAttack();
}
void ABaseEnemy::BeHit()
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(GetController());
	control->OnBeHit();
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

void ABaseEnemy::AttachEquipment()
{
	FTransform swordTr = GetSKMesh()->GetSocketTransform("Sword_socket", RTS_World);
	FTransform shieldTr = GetSKMesh()->GetSocketTransform("Shield_socket", RTS_World);

	swordMesh->AttachToComponent(GetSKMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Sword_socket");
	shieldMesh->AttachToComponent(GetSKMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), "Shield_socket");

	swordMesh->SetWorldTransform(swordTr);
	shieldMesh->SetWorldTransform(shieldTr);
}

