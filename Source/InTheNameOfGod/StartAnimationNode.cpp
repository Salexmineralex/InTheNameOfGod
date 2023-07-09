// Fill out your copyright notice in the Description page of Project Settings.


#include "StartAnimationNode.h"

#include "EdGraphSchema_K2.h"

#include "EdGraph/EdGraphPin.h"

void UStartAnimationNode::AllocateDefaultPins()
{
	// Create the execution output pins
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TEXT("Output1"));
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, TEXT("Output2"));
}

FText UStartAnimationNode::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	// Customize the display name based on the title type
	if (TitleType == ENodeTitleType::ListView)
	{
		return FText::FromString(TEXT("PAPOPEPO"));
	}
	else
	{
		return FText::FromString(TEXT("PAPOPEPO"));
	}
}

FText UStartAnimationNode::GetPinDisplayName(const UEdGraphPin* Pin) const
{
	// Customize the display name for execution pins
	if (Pin->Direction == EGPD_Output && Pin->PinType.PinCategory == UEdGraphSchema_K2::PC_Exec)
	{
		if (Pin->PinName == TEXT("Output1"))
		{
			return FText::FromString(TEXT("Exec 1"));
		}
		else if (Pin->PinName == TEXT("Output2"))
		{
			return FText::FromString(TEXT("Exec 2"));
		}
	}

	return Super::GetPinDisplayName(Pin);
}


void UStartAnimationNode::StartAnimation()
{
	// Implement your custom logic here
}
