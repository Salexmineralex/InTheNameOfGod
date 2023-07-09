// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "WayPoint.generated.h"

/**
 * 
 */
UCLASS()
class INTHENAMEOFGOD_API AWayPoint : public ATargetPoint
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	int32 idPosition{0};
};
