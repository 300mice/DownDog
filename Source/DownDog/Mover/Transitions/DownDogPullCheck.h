// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosMover/ChaosMovementModeTransition.h"
#include "DownDogPullCheck.generated.h"

/**
 * 
 */
UCLASS()
class DOWNDOG_API UDownDogPullCheck : public UChaosMovementModeTransition
{
	GENERATED_BODY()

	virtual FTransitionEvalResult Evaluate_Implementation(const FSimulationTickParams& Params) const override;

public:
	
	// Name of movement mode to transition to when pulling an object.
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Parameters)
	FName PullingModeName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Parameters)
	FName WalkingModeName;
	
};
