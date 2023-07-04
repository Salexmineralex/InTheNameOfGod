// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "EnhancedInputComponent.h"
#include "LifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "InTheNameOfGod/Enemies/FollowEnemiesPoints.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ChildActor = CreateDefaultSubobject<UChildActorComponent>(TEXT("Weapon"));

	ChildActor->bEditableWhenInherited = true;
    
    ChildActor->CreateChildActor();
	//
	// swordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("swordMesh"));
	// swordMesh->SetupAttachment(RootComponent);
	//
	// swordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("swordCollision"));
	// swordCollision->SetupAttachment(swordMesh);
	
	// niagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("Niagara Component"));
	
	followableComponent = CreateDefaultSubobject<UFollowEnemiesPoints>(TEXT("Followable component"));
	
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
	


}

// Called every frame
void AMainPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

		//Action
		EnhancedInputComponent->BindAction(BuffAction, ETriggerEvent::Started, this, &AMainPlayer::PlayBuffAnim);
		

	}



}

void AMainPlayer::Move(const FInputActionValue& Value)
{
	Super::Move(Value);

	FVector2d VectorMovement = Value.Get<FVector2d>();
	
	if(VectorMovement.Y > 0 && GetCurrentMontage() != walkAnimMontage)
	{
		PlayAnimMontage(this->walkAnimMontage);	
	}
	
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

void AMainPlayer::Secondary_Attack(const FInputActionValue& Value)
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

	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Yellow, FString::Printf(TEXT("Inputs NUM -> %d"), inputs.Num()));
	
	if(inputs.Num() == 0 && montage)
	{
		canAttack = false;
		animationBeenPlayed = true;
		PlayAnimMontage(montage);
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

	TSubclassOf<ULifeComponent> LifeComponent;
	ULifeComponent* life =  Cast<ULifeComponent>(OtherActor->GetComponentByClass(LifeComponent));
	life = OtherActor->FindComponentByClass<ULifeComponent>();
	
	if(life && canAttack)
	{
		StartHitStop();
		life->GetDamage(actualWeapon->Damage()+(actualWeapon->Damage()*multiplayerDamage*HasWeapon));
	}

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
	Super::Jump();

	isJumping = true;

	GetWorld()->GetTimerManager().SetTimer(stopJump,this, &AMainPlayer::StopJumping, 0.1f, false,0.1f);

}

void AMainPlayer::StopJumping()
{
	Super::StopJumping();

	isJumping = false;
}

void AMainPlayer::Dash()
{
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Dash"));
	if(!GetCharacterMovement()->IsFalling() && canDash)
	{
		LaunchCharacter( GetActorUpVector()*200+GetActorForwardVector()*600,false,false);
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,niagaraDash,GetActorLocation(),UKismetMathLibrary::Conv_VectorToRotator(UKismetMathLibrary::GetForwardVector(GetActorRotation())*-1),FVector(0.75f));
		canDash = false;
		FTimerDelegate DashDelegate = FTimerDelegate::CreateUObject( this, &AMainPlayer::SetCanDash, true );
		GetWorldTimerManager().SetTimer( dashTimer, DashDelegate, 2, false );
		
	}

}

void AMainPlayer::PlayBuffAnim()
{
	if(!HasWeapon)
	{
		AttachWeapon();
	}
	PlayAnimMontage(buffSwordAnimationMontage);
}

void AMainPlayer::BuffSword()
{
	
	if(!isBuffed)
	{
		isBuffed = true;
		actualWeapon->swordMesh->SetMaterial(0,actualWeapon->buffSwordMaterial);
		GetWorld()->GetTimerManager().SetTimer(buffTimer,this, &AMainPlayer::BuffSword, buffTime, false,buffTime);

	}else
	{
		isBuffed = false;
		actualWeapon->swordMesh->SetMaterial(0,actualWeapon->normalSwordMaterial);
	}
	
}



