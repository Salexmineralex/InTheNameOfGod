// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "StartAnimationNode.generated.h"

/**
 * 
 */
UCLASS()
class INTHENAMEOFGOD_API UStartAnimationNode : public UK2Node
{
	GENERATED_BODY()

	

public:


	UFUNCTION(BlueprintCallable, Category = "CustomNode")
	void StartAnimation();
	
	virtual void AllocateDefaultPins() override;
	// Override the GetNodeTitle function to customize the display name
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	// Override the GetPinName function to customize pin names
	virtual FText GetPinDisplayName(const UEdGraphPin* Pin) const override;
};
