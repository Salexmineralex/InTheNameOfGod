// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseEnemy.h"
//unreal
#include "Components/SkeletalMeshComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Components/WidgetComponent.h"
//project
#include "AI_BaseEnemyAnimation.h"
#include "BaseEnemyController.h"
#include "PickableObject.h"
#include "InTheNameOfGod/LifeComponent.h"
#include "InTheNameOfGod/AI/WayPoint.h"
#include "InTheNameOfGod/MainPlayer.h"







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
	swordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("SwordCollision"));
	swordCollision->SetupAttachment(swordMesh);
	swordCollision->OnComponentBeginOverlap.AddDynamic(this, &ABaseEnemy::DamagePlayer);
	swordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	
	whiteballComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("whiteBall"));
	whiteballComponent->SetupAttachment(RootComponent);
	whiteballComponent->SetWidgetSpace(EWidgetSpace::Screen);

}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();
	whiteballComponent->SetVisibility(false);
	AttachEquipment();
	controller = Cast<ABaseEnemyController>(GetController());
	//if (ABaseEnemyController* control = Cast<ABaseEnemyController>(GetController()))
	//{
	//	if (!control->HaveCalledBeginPlay())
	//	{
	//		control->CallBeginPlay();
	//	}
	//}
	//else
	//{
	//	FActorSpawnParameters SpawnParams;
	//	SpawnParams.Owner = this;
	//	//SpawnParams.Instigator = Instigator;
	//	//AIControllerClass = AIControllerEnemyClass.StaticClass();
	//	ABaseEnemyController* controller = GetWorld()->SpawnActor<ABaseEnemyController>(AIControllerEnemyClass, GetActorLocation(), GetActorRotation(), SpawnParams);
	//	controller->Possess(this);
	//	controller->CallBeginPlay();//aqui da una excepcion nose porque pero rula igualmente
	//}

}
void ABaseEnemy::BeUnderAttack()
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(GetController());
	control->OnReciveAttack(32);
}
void ABaseEnemy::BeHit()
{
	ABaseEnemyController* control = Cast<ABaseEnemyController>(GetController());
	control->OnBeHit(32);
}

// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(controller)
	{
		if(controller->abpEnemy)
		{
			if(!controller->abpEnemy->GetCurrentActiveMontage())
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Toma Mango")));
				swordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
			}
		}
	
	}
	

}

// Called to bind functionality to input
void ABaseEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
void ABaseEnemy::OnDie()
{
	if (ABaseEnemyController* Enemycontroller = Cast<ABaseEnemyController>(GetController()))
	{
		Enemycontroller->OnEnemyDie();
		if(FMath::RandRange(0, 100) <= 35)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

			FTransform SpawnTransform;
			SpawnTransform.SetLocation(GetActorLocation());
			if(FMath::RandRange(0, 100) <= 50)
			{
				APickableObject* SpawnedObject = GetWorld()->SpawnActor<APickableObject>(Mana_Potion, SpawnTransform, SpawnParams);
			}else
			{
				APickableObject* SpawnedObject = GetWorld()->SpawnActor<APickableObject>(Life_Potion, SpawnTransform, SpawnParams);
			}
		}
		
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

void ABaseEnemy::DamagePlayer(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (AMainPlayer* playerHit = Cast<AMainPlayer>(OtherActor))
	{
		GetWorld()->GetTimerManager().SetTimer(swordCollisionTimerHandle,this, &ABaseEnemy::Recover_Sword, 0.20f, false,0.20f);
		swordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		playerHit->GetDamage(damage);
	}
}

void ABaseEnemy::Recover_Sword()
{
	swordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}
