// Fill out your copyright notice in the Description page of Project Settings.


#include "MainPlayer.h"

#include "EnhancedInputComponent.h"

// Sets default values
AMainPlayer::AMainPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	this->swordMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("swordMesh"));
	swordMesh->SetupAttachment(RootComponent);
	
	

}

// Called when the game starts or when spawned
void AMainPlayer::BeginPlay()
{
	Super::BeginPlay();

	FTransform swordtr =  GetMesh()->GetSocketTransform("sword_socket",RTS_World);
	

	swordMesh->AttachToComponent(GetMesh(),FAttachmentTransformRules(EAttachmentRule::KeepRelative,true),"sword_socket");
	
	swordMesh->SetWorldTransform(swordtr);
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
	if(GetCurrentMontage() != Attack1AnimMontage)
	{
		PlayAnimMontage(this->Attack1AnimMontage);
	}

	
}

void AMainPlayer::Secondary_Attack(const FInputActionValue& Value)
{
	
	if(GetCurrentMontage() != Attack2AnimMontage)
	{
		PlayAnimMontage(this->Attack2AnimMontage);
	}

}


