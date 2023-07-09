// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObject.h"

// Sets default values
APickableObject::APickableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objectMesh = CreateDefaultSubobject<UMeshComponent>(TEXT("objectMesh"));

	objectCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("objectCollision"));
	objectCollision->SetupAttachment(objectMesh);



}

// Called when the game starts or when spawned
void APickableObject::BeginPlay()
{
	Super::BeginPlay();
	objectCollision->OnComponentBeginOverlap.AddDynamic(this,&APickableObject::OnPickUp);
}

// Called every frame
void APickableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickableObject::OnPickUp(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	APawn* Enemy = Cast<APawn>(OtherActor);
	
	if(pawnThatCanPickUP* target = Cast<pawnThatCanPickUP>(OtherActor))
	{
		
	}
}

