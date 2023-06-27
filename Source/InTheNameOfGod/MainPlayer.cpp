// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "EnhancedInputComponent.h"
#include "LifeComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
//project
#include "GameFramework/SpringArmComponent.h"
#include "InTheNameOfGod/Enemies/FollowEnemiesPoints.h"


// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->swordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("swordMesh"));
	swordMesh->SetupAttachment(RootComponent);

	this->swordCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("swordCollision"));
	this->swordCollision->SetupAttachment(swordMesh);

	followableComponent = CreateDefaultSubobject<UFollowEnemiesPoints>(TEXT("Followable component"));




}

// Called when the game s or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	FTransform swordtr =  GetMesh()->GetSocketTransform("sword_socket",RTS_World);
	
	swordMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),"sword_socket");
	
	swordMesh->SetWorldTransform(swordtr);
	
	this->swordCollision->OnComponentBeginOverlap.AddDynamic(this,&AMainPlayer::DamageEnemy);

	this->swordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

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
	StopAnimMontage(GetCurrentMontage());
	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("StopMoving"));
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
		// if(animationBeenPlayed == false && inputArray.Num() > 1)
		// {
		// 	inputArray.Reset();
		// 	
		// }
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
		// if(animationBeenPlayed == false && inputArray.Num() > 1)
		// {
		// 	inputArray.Reset();
		// 		
		// }
		copyinput.Append(inputArray);
		StartCombo_Implementation(copyinput);
	}
	

}

void AMainPlayer::StartCombo_Implementation(const TArray<EAttackInputCombo> &inputsArray)
{
	// canAttack = true;
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
		life->GetDamage(10);
	}

}

void AMainPlayer::StartHitStop()
{
	this->CustomTimeDilation = 0;
	
	GetWorld()->GetTimerManager().SetTimer(hitStop,this, &AMainPlayer::EndHitStop, 0.16f, false,0.16f);
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
		
		swordMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::SnapToTarget,false),"sword_scabbard");
	
		swordMesh->SetWorldTransform(swordtr);

		HasWeapon = false;
	}else
	{
		FTransform swordtr =  GetMesh()->GetSocketTransform("sword_socket",RTS_World);
	
		swordMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),"sword_socket");
	
		swordMesh->SetWorldTransform(swordtr);

		HasWeapon = true;
	}
}



