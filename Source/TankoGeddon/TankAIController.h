// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Scorable.h"
#include "TankAIController.generated.h"

/**
 * 
 */
UCLASS()
class TANKOGEDDON_API ATankAIController : public AAIController, public IScorable
{
	GENERATED_BODY()
	
protected:
	UPROPERTY()
	class ATankPawn* TankPawn;

	UPROPERTY()
	class APawn* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement")
	TArray<FVector> PattrollingPath;

	int32 CurrentPattrolingIndex = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Movement")
	float MovementAccurency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Targeting")
	float TargetingRange = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Targeting")
	float TargetingSpeed = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI | Targeting")
	float Accurency = 10.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Score")
	int ScorePoints = 300;

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	float GetRotationValue();
	void Targeting();
	void RotateToPlayer();
	bool IsPlayerRange();
	bool CanFire();
	void Fire();
	int GetPoint() const;

	bool IsPlayerSeen();
};
