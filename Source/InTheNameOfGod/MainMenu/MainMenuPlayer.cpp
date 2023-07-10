// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuPlayer.h"
#include "UW_MenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMainMenuPlayer::AMainMenuPlayer()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	firstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	firstPersonCamera->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMainMenuPlayer::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
	mainMenuWidgetInstance = CreateWidget<UUW_MenuWidget>(GetWorld(), mainMenuWidget);
	mainMenuWidgetInstance->AddToViewport();
	mainMenuWidgetInstance->levelToLoad = levelToLoad;
}

// Called every frame
void AMainMenuPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMainMenuPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

