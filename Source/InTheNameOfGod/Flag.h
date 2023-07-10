// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Flag.generated.h"


class AMainPlayer;
UCLASS()
class INTHENAMEOFGOD_API AFlag : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFlag();

	void RaisedFlag();
	void LowerFlag();

	UFUNCTION()
		void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex,
			bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
		void EndOverlap(UPrimitiveComponent* OverlappedComponent,
			AActor* OtherActor,
			UPrimitiveComponent* OtherComp,
			int32 OtherBodyIndex);

	void StartFlagMovingDown(float Duration);
	void StartFlagMovingUp(float Duration);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;




public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* FlagPoleMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	UStaticMeshComponent* FlagMesh;

	UPROPERTY(EditAnywhere, Category = "Components")
	USphereComponent* RangeSphere;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	TArray<AActor*> Enemies;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float PlayerProximityRange;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float EnemyProximityRange;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	float FlagsUp;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
	UMaterialInterface* RaisedFlagMaterial;

	UPROPERTY(EditAnywhere, Category = "Gameplay")
		UMaterialInterface* LoweredFLagMaterial;

	FVector TargetFlagLocation;
	FVector InitialFlagLocation;
	float StartTime;
	float MovementDuration;

	bool bIsRaised;
	bool bIsFlagMoving;
	bool bIsOverlaping;
	bool bIsFlagDown;
	bool bIsFlagGoingDown;

	AMainPlayer* Player;
	
};
