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
    bIsFlagGoingDown= false;
    bIsFlagLocked = false;

    FVector StartFlagLocation = FlagMesh->GetRelativeLocation();
    StartFlagLocation.Z = 70.f;
    FlagMesh->SetWorldLocation(StartFlagLocation);
}

// Called when the game starts or when spawned
void AFlag::BeginPlay()
{
	Super::BeginPlay();

    ACharacter* player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    Player = player;
    FVector StartFlagLocation = FlagMesh->GetRelativeLocation();
    StartFlagLocation.Z = 70.f;
}

// Called every frame
void AFlag::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bIsOverlaping)
    {
        if (!bIsFlagMoving && !bIsFlagDown && !bIsFlagLocked)
        {
            StartFlagMovingDown(10.f);
            
        }
        else
        {
            float ElapsedTime = GetWorld()->GetTimeSeconds() - StartTime;
            float Alpha = ElapsedTime / MovementDuration;
            
           
            if (ElapsedTime >= MovementDuration)
            {
                bIsFlagMoving = false;
                bIsFlagDown = true;
            }

            if (bIsFlagGoingDown)
            {
                FVector NewRelativeLocation = FMath::Lerp(InitialFlagLocation, TargetFlagLocation, Alpha);
                FVector NewWorldLocation = GetActorTransform().TransformPositionNoScale(NewRelativeLocation);
                FlagMesh->SetWorldLocation(NewWorldLocation);
            }

            if (bIsFlagDown && !bIsFlagLocked)
            {
                bIsFlagMoving = true;
                StartFlagMovingUp(10.f);
                bIsFlagLocked = true;

            }
            else
            {
                float ElapsedTimeTwo = GetWorld()->GetTimeSeconds() - StartTime;
                float AlphaTwo = ElapsedTime / MovementDuration;

                if (ElapsedTimeTwo >= MovementDuration)
                {
                    bIsFlagMoving = false;
                    bIsRaised = true;
                }

                FVector NewRelativeLocation = FMath::Lerp(TargetFlagLocation, InitialFlagLocation, Alpha);
                FVector NewWorldLocation = GetActorTransform().TransformPositionNoScale(NewRelativeLocation);
                FlagMesh->SetWorldLocation(NewWorldLocation);
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
        FVector TargetLocation = FlagMesh->GetRelativeLocation() + FVector(0.0f, 0.0f, 70);
        InitialFlagLocation = FlagMesh->GetRelativeLocation();
        StartTime = GetWorld()->GetTimeSeconds();
        MovementDuration = Duration;
        bIsFlagGoingDown = true;
        
    }
}
void AFlag::StartFlagMovingUp(float Duration)
{
    if (!bIsFlagMoving)
    {
        bIsFlagMoving = true;
        FVector TargetLocation = FlagMesh->GetRelativeLocation() - FVector(0.0f, 0.0f, 70);
        InitialFlagLocation = FlagMesh->GetRelativeLocation();
        StartTime = GetWorld()->GetTimeSeconds();
        MovementDuration = Duration;

    }
}


