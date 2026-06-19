// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosMover/Character/Modes/ChaosWalkingMode.h"
#include "DownDogWalkingMode.generated.h"

/**
 * 
 */
UCLASS()
class DOWNDOG_API UDownDogWalkingMode : public UChaosWalkingMode
{
	GENERATED_BODY()

	virtual float GetMaxSpeed() const override;
	
};
