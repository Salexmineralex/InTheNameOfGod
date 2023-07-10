// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "EnhancedInputComponent.h"
#include "LifeComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/WidgetComponent.h"
#include "Enemies/BaseEnemy.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "InTheNameOfGod/Enemies/FollowEnemiesPoints.h"
#include "Kismet/KismetMathLibrary.h"
#include "Enemies/BaseEnemy.h"
#include "Enemies/BaseEnemyController.h"
#include "UI/UIW_PlayerHUD.h"



// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));

	ChildActor->bEditableWhenInherited = true;
    
    ChildActor->CreateChildActor();
	
	followableComponent = CreateDefaultSubobject<UFollowEnemiesPoints>(TEXT("Followable component"));

	//followableComponent->bWantsBeginPlay = true;

	enemyPointsParent = CreateDefaultSubobject<USceneComponent>(TEXT("EnemyPointsParent"));
	enemyPointsParent->SetupAttachment(RootComponent);
	followableComponent->parentPoints = enemyPointsParent;

	lifeComponent = CreateDefaultSubobject<ULifeComponent>(TEXT("Life Component"));
}

// Called when the game s or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	ChildActor->RegisterComponent();
	
	FTransform swordtr =  GetMesh()->GetSocketTransform("sword_socket",RTS_World);
	
	actualWeapon = Cast<AWeapon>(ChildActor->GetChildActor());

	if(actualWeapon)
	{
		actualWeapon->swordMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),"sword_socket");
	
		actualWeapon->swordMesh->SetWorldTransform(swordtr);
	
		actualWeapon->swordCollision->OnComponentBeginOverlap.AddDynamic(this,&AMainPlayer::DamageEnemy);

		actualWeapon->swordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	

	playerWidget = CreateWidget<UUIW_PlayerHUD>(GetWorld(), playerWidgetType);
	playerWidget->AddToViewport();
	playerWidget->LifeBar()->SetPercent(lifeComponent->GetLifePercent());
	playerWidget->ManaBar()->SetPercent(actualMana/maxMana);

}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	if(enemyTarget)
	{
		GetController()->SetControlRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),enemyTarget->GetActorLocation()-FVector(0,0,100)));
		// GetMesh()->SetWorldRotation(UKismetMathLibrary::FindLookAtRotation(GetActorLocation(),enemyTarget->GetActorLocation())-FRotator(0,90,0));
		if(ABaseEnemy* Enemy = Cast<ABaseEnemy>(enemyTarget))
		{
			if(Enemy->lifeComponent->currentLife <= 0)
			{
				LockEnemy();
			}
			
		}
	}
	

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	MyPlayerInputComponent = PlayerInputComponent;
	//Moving
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//StopMoving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &AMainPlayer::StopMoving);

		//Attack
		EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &AMainPlayer::Attack);

		//Secondary Attack
		EnhancedInputComponent->BindAction(SecondaryAction, ETriggerEvent::Started, this, &AMainPlayer::Secondary_Attack);

		//Dash
		EnhancedInputComponent->BindAction(DashAction, ETriggerEvent::Completed, this, &AMainPlayer::Dash);

		//Buff
		EnhancedInputComponent->BindAction(BuffAction, ETriggerEvent::Started, this, &AMainPlayer::PlayBuffAnim);

		//Lock
		EnhancedInputComponent->BindAction(LockAction, ETriggerEvent::Started, this, &AMainPlayer::LockEnemy);
		

	}



}

void AMainPlayer::Move(const FInputActionValue& Value)
{
	Super::Move(Value);
	//
	//
	// FVector2d VectorMovement = Value.Get<FVector2d>();
	//
	// if(VectorMovement.Y > 0 && GetCurrentMontage() != walkAnimMontage)
	// {
	// 	PlayAnimMontage(this->walkAnimMontage);	
	// }

	
}


void AMainPlayer::StopMoving(const FInputActionValue& Value)
{
	if(GetCurrentMontage())
	{
		canAttack = true;
	}
	StopAnimMontage(GetCurrentMontage());

}

void AMainPlayer::Attack(const FInputActionValue& Value)
{
	if(canAttacked)
	{
		if(animationBeenPlayed == false && inputArray.Num() >= 1)
		{
			inputArray.Reset();
				
		}
	
		if(canAttack)
		{
			inputArray.Push(EAttackInputCombo::Light);
			TArray<EAttackInputCombo> copyinput;
			copyinput.Append(inputArray);
			StartCombo_Implementation(copyinput);
		
		}
	}

}

void AMainPlayer::Secondary_Attack(const FInputActionValue& Value)
{
	if(canAttacked)
	{
		if(animationBeenPlayed == false && inputArray.Num() >= 1)
		{
			inputArray.Reset();
				
		}
	
	
		if(canAttack)
		{
			inputArray.Push(EAttackInputCombo::Strong);
			TArray<EAttackInputCombo> copyinput;
			copyinput.Append(inputArray);
			StartCombo_Implementation(copyinput);
		}
	}

	

}

void AMainPlayer::StartCombo_Implementation(const TArray<EAttackInputCombo> &inputsArray)
{
	// canAttack = true;
	if(!HasWeapon)
	{
		AttachWeapon();
		
	}
	GetWorld()->GetTimerManager().ClearTimer(attachWeapon);
	animationBeenPlayed = false;

	StartCombo(inputsArray);
	
}




void AMainPlayer::SelectAnimationByInput(TArray<EAttackInputCombo> inputs,UAnimMontage* montage, EAttackInputCombo& input,TArray<EAttackInputCombo>& outputInput)
{

	
	if(inputs.Num() == 0 && montage)
	{
		canAttack = false;
		animationBeenPlayed = true;
		PlayAnimMontage(montage);
		actualWeapon->swordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		input = EAttackInputCombo::defaults;
		GetWorld()->GetTimerManager().SetTimer(attachWeapon,this, &AMainPlayer::AttachAnimation, 5.0f, false,5.0f);


	}

	if( inputs.Num() > 0)
	{
		canAttack = true;
		input = inputs[0];
		inputs.RemoveAt(0);
		actualInputs = inputs.Num();
		outputInput = inputs;

	}

}


void AMainPlayer::DamageEnemy(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{


	if(ABaseEnemy* Enemy = Cast<ABaseEnemy>(OtherActor))
	{
		TSubclassOf<ULifeComponent> LifeComponent;
		ULifeComponent* life =  Cast<ULifeComponent>(OtherActor->GetComponentByClass(LifeComponent));
		life = OtherActor->FindComponentByClass<ULifeComponent>();
			if (ABaseEnemyController* control = Cast<ABaseEnemyController>(Enemy->GetController()))
			{
				StartHitStop();
				actualWeapon->swordCollision->Deactivate();
				GetWorld()->GetTimerManager().SetTimer(swordCollision,this, &AMainPlayer::Recover_Sword, 0.20f, false,0.20f);

				control->OnReciveAttack(actualWeapon->Damage()+(actualWeapon->Damage()*multiplayerDamage)*(float)isBuffed);
			}
			
	}
	
}

void AMainPlayer::Recover_Sword()
{
	actualWeapon->swordCollision->Activate();
}

void AMainPlayer::StartHitStop()
{
	this->CustomTimeDilation = 0;
	
	GetWorld()->GetTimerManager().SetTimer(hitStop,this, &AMainPlayer::EndHitStop, 0.07f, false,0.07f);
}

void AMainPlayer::EndHitStop()
{
	this->CustomTimeDilation = 1;
	
}


void AMainPlayer::AttachAnimation()
{
	PlayAnimMontage(attachAnimationMontage);
}

void AMainPlayer::AttachWeapon()
{

	if(HasWeapon)
	{
		FTransform swordtr =  GetMesh()->GetSocketTransform("sword_scabbard",RTS_World);
		
		actualWeapon->swordMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false),"sword_scabbard");
	
		actualWeapon->swordMesh->SetWorldTransform(swordtr);

		HasWeapon = false;
	}else
	{
		FTransform swordtr =  GetMesh()->GetSocketTransform("sword_socket",RTS_World);
	
		actualWeapon->swordMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),"sword_socket");
	
		actualWeapon->swordMesh->SetWorldTransform(swordtr);

		HasWeapon = true;
	}
}

void AMainPlayer::Jump()
{
	if(canJumped)
	{
		Super::Jump();

		isJumping = true;

		GetWorld()->GetTimerManager().SetTimer(stopJump,this, &AMainPlayer::StopJumping, 0.1f, false,0.1f);

	}
	
}

void AMainPlayer::StopJumping()
{
	Super::StopJumping();

	isJumping = false;
}

void AMainPlayer::Dash()
{
	if(canDashed)
	{
		if(!GetCharacterMovement()->IsFalling() && canDash)
		{
			LaunchCharacter( GetActorUpVector()*200+GetActorForwardVector()*600,false,false);
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,niagaraDash,GetActorLocation(),UKismetMathLibrary::Conv_VectorToRotator(UKismetMathLibrary::GetForwardVector(GetActorRotation())*-1),FVector(0.75f));
		
			canDash = false;
			FTimerDelegate DashDelegate = FTimerDelegate::CreateUObject( this, &AMainPlayer::SetCanDash, true );
			GetWorldTimerManager().SetTimer( dashTimer, DashDelegate, 2, false );
		
		}
	}


}

void AMainPlayer::PlayBuffAnim()
{
	if(canBuffed)
	{
		if(actualMana >= 35)
		{
		
			if(!HasWeapon)
			{
				AttachWeapon();
			}
			PlayAnimMontage(buffSwordAnimationMontage);
		}
	}


}

void AMainPlayer::BuffSword()
{
	
	if(!isBuffed)
	{
		actualWeapon->niagaraBuffed->SetVisibility(true);
		isBuffed = true;
		SubstractMana(35);
		actualWeapon->swordMesh->SetMaterial(0,actualWeapon->buffSwordMaterial);
		GetWorld()->GetTimerManager().SetTimer(buffTimer,this, &AMainPlayer::BuffSword, buffTime, false,buffTime);
		
	}else
	{
		actualWeapon->niagaraBuffed->SetVisibility(false);
		isBuffed = false;
		actualWeapon->swordMesh->SetMaterial(0,actualWeapon->normalSwordMaterial);
	}
	
}


void AMainPlayer::LockEnemy()
{
	if(canLocked)
	{
		if(!enemyLocked)
		{
			enemyLocked = true;
			UWorld* world = GetWorld();
			FVector start = GetActorLocation();
			FVector end = (UKismetMathLibrary::GetForwardVector(GetFollowCamera()->GetComponentRotation())*5000)+GetActorLocation();
			TArray<TEnumAsByte<EObjectTypeQuery>> objectTypesArray; // object types to trace
			objectTypesArray.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
			ETraceTypeQuery trace = ETraceTypeQuery::TraceTypeQuery1;
			auto draw = EDrawDebugTrace::None;
			FHitResult hit = FHitResult();
			FLinearColor color = FLinearColor(0.4f,0.5f,0.4f,1);
			FLinearColor colorhit = FLinearColor(0.9f,0.4,0.4f,1);
		
			if(UKismetSystemLibrary::SphereTraceSingleForObjects(world,start,end,125,objectTypesArray,false,{},draw,hit,true,color,colorhit))
			{
				enemyTarget = hit.GetActor();
				if(ABaseEnemy* Enemy = Cast<ABaseEnemy>(enemyTarget))
				{
					Enemy->whiteballComponent->SetVisibility(true);
			
				}

				GetController()->SetIgnoreLookInput(true);
				GetCharacterMovement()->bOrientRotationToMovement = false;
				GetCharacterMovement()->bUseControllerDesiredRotation = true;
	
			}
		
		}else
		{
			if(ABaseEnemy* Enemy = Cast<ABaseEnemy>(enemyTarget))
			{
			
				Enemy->whiteballComponent->SetVisibility(false);
			}
		
			GetController()->ResetIgnoreLookInput();
			GetCharacterMovement()->bOrientRotationToMovement = true;
			GetCharacterMovement()->bUseControllerDesiredRotation = false;
			enemyTarget = nullptr;
			enemyLocked = false;
		}
	}
	
}

void AMainPlayer::GetDamage(float damage)
{
	lifeComponent->GetDamage(damage);
	playerWidget->LifeBar()->SetPercent(lifeComponent->GetLifePercent());
}
void AMainPlayer::GetHeal(float heal)
{
	lifeComponent->GetHeal(heal);
	playerWidget->LifeBar()->SetPercent(lifeComponent->GetLifePercent());
}



