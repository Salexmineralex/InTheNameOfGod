// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MenuWidget.generated.h"

/**
 * 
 */

class UButton;
UCLASS()
class INTHENAMEOFGOD_API UUW_MenuWidget : public UUserWidget
{
	GENERATED_BODY()
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void ExitGame();
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* startGameButton = nullptr;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UButton* exitgameButton = nullptr;

	FName levelToLoad;
};
