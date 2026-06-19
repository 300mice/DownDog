// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DownDogCharacter.h"
#include "ChaosMover/Character/Modes/ChaosWalkingMode.h"
#include "DownDogPullingMode.generated.h"

/**
 * 
 */
UCLASS()
class DOWNDOG_API UDownDogPullingMode : public UChaosWalkingMode
{
	GENERATED_BODY()

	virtual float GetMaxSpeed() const override;

	virtual void Activate() override;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PullingControl")
	float MaximumDistanceFromObject;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "PullingControl")
	float PullForce;

private:

	UPROPERTY()
	TObjectPtr<AItem> HeldItem;
	UPROPERTY()
	TObjectPtr<ADownDogCharacter> DownDogCharacter;
	UPROPERTY()
	TObjectPtr<UAnchorLocation> ClosestAnchor;
	
};
