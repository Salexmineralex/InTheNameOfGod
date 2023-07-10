// Fill out your copyright notice in the Description page of Project Settings.


#include "Flag.h"
#include "MainPlayer.h"

// Sets default values
AFlag::AFlag()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    FlagPoleMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagPoleMesh"));
    RootComponent = FlagPoleMesh;

    FlagMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlagMesh"));
    FlagMesh->SetupAttachment(RootComponent);
 

    RangeSphere = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphere"));
    RangeSphere->SetupAttachment(RootComponent);
    RangeSphere->SetSphereRadius(1000.0f);

    RangeSphere->OnComponentBeginOverlap.AddDynamic(this, &AFlag::BeginOverlap);
    RangeSphere->OnComponentEndOverlap.AddDynamic(this, &AFlag::EndOverlap);

    bIsRaised = false;
    bIsFlagMoving = false;
    bIsOverlaping = false;
    bIsFlagDown= false;
    bIsFlagGoingDown;

    FVector StartFlagLocation = FlagMesh->GetComponentLocation();
    StartFlagLocation.Z = 10.f;
    FlagMesh->SetWorldLocation(StartFlagLocation);
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsOverlaping)
    {
        if (!bIsFlagMoving)
        {
            StartFlagMovingDown(10.f);
        }
        else
        {
            float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
            float Alpha = ElapsedTime / MovementDuration;
            FVector NewLocation;
            if (bIsFlagGoingDown)
            {
                NewLocation = FMath::Lerp(InitialFlagLocation, TargetFlagLocation, Alpha);
            }
            else
            {
                NewLocation = FMath::Lerp(TargetFlagLocation, InitialFlagLocation, Alpha);
            }
            FlagMesh->SetWorldLocation(NewLocation);
            if (ElapsedTime >= MovementDuration)
            {
                bIsFlagMoving = false;
                bIsFlagDown = true;
            }
            if (bIsFlagDown)
            {
                bIsFlagMoving = true;
                StartFlagMovingUp(10.f);

            }
        }
       
    }

}


void AFlag::BeginOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex,
    bool bFromSweep,
    const FHitResult& SweepResult)

{
    if (OtherActor == Player && Enemies.Num() == 0)
    {
        bIsOverlaping = true;
    }
}

void AFlag::EndOverlap(UPrimitiveComponent* OverlappedComponent,
    AActor* OtherActor,
    UPrimitiveComponent* OtherComp,
    int32 OtherBodyIndex)

{
    if (bIsRaised)
    {
        FlagsUp++;
    }
}


void AFlag::StartFlagMovingDown(float Duration)
{
    if (!bIsFlagMoving)
    {
        bIsFlagMoving = true;
        FVector TargetLocation = FlagMesh->GetComponentLocation() - FVector(0.0f, 0.0f, 10);
        InitialFlagLocation = FlagMesh->GetComponentLocation();
        StartTime = GetWorld()->GetTimeSeconds();
        MovementDuration = Duration;
        bIsFlagDown = true;
        
    }
}
void AFlag::StartFlagMovingUp(float Duration)
{
    if (!bIsFlagMoving)
    {
        bIsFlagMoving = true;
        FVector TargetLocation = FlagMesh->GetComponentLocation() + FVector(0.0f, 0.0f, 10);
        InitialFlagLocation = FlagMesh->GetComponentLocation();
        StartTime = GetWorld()->GetTimeSeconds();
        MovementDuration = Duration;

    }
}


