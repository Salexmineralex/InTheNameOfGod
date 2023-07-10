// Fill out your copyright notice in the Description page of Project Settings.


#include "PickableObject.h"

#include "GameFramework/Character.h"

// Sets default values
APickableObject::APickableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	objectMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("objectMesh"));
	SetRootComponent(objectMesh);
	objectCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("objectCollision"));
	objectCollision->SetupAttachment(RootComponent);



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

	if(OtherActor->IsA(pawnThatCanPickUP))
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, FString::Printf(TEXT("OVerlpaed")));

		Pickuped(OtherActor);
	}
}

