// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/RichTextBlock.h"
#include "Components/TextBlock.h"
#include "UIW_PlayerHUD.generated.h"

class UProgressBar;

UCLASS()
class INTHENAMEOFGOD_API UUIW_PlayerHUD : public UUserWidget
{
	GENERATED_BODY()
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//UProgressBar* lifeBar{nullptr};
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//UProgressBar* manaBar{nullptr};
protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* lifeBar{nullptr};

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UProgressBar* manaBar{nullptr};
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	URichTextBlock* textTutorial{nullptr};
public:


	[[nodiscard]] UProgressBar* LifeBar() const
	{
		return lifeBar;
	}

	void SetLifeBar(UProgressBar* LifeBar)
	{
		lifeBar = LifeBar;
	}

	[[nodiscard]] UProgressBar* ManaBar() const
	{
		return manaBar;
	}

	void SetManaBar(UProgressBar* ManaBar)
	{
		manaBar = ManaBar;
	}
};
