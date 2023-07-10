// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuPlayer.generated.h"

class UCameraComponent;
class UUW_MenuWidget;

UCLASS()
class INTHENAMEOFGOD_API AMainMenuPlayer : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AMainMenuPlayer();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditDefaultsOnly) TSubclassOf<UUW_MenuWidget> mainMenuWidget;
	UPROPERTY() UUW_MenuWidget* mainMenuWidgetInstance = nullptr;

	UPROPERTY(EditDefaultsOnly)
		UCameraComponent* firstPersonCamera = nullptr;

	UPROPERTY(EditDefaultsOnly)
		FName levelToLoad;

};
