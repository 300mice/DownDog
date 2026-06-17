// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ChaosMover/Character/ChaosCharacterMoverComponent.h"
#include "DownDogChaosMoverComponent.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable, meta = (BlueprintSpawnableComponent))
class DOWNDOG_API UDownDogChaosMoverComponent : public UChaosCharacterMoverComponent
{
	GENERATED_BODY()

public:
	
	UFUNCTION(BlueprintCallable, Category = "Mover")
	void AddExternalForce(FVector Force);

	virtual void ProduceInput(const int32 DeltaTimeMS, FMoverInputCmdContext* Cmd) override;

private:
	FVector AccumulatedExternalForce = FVector::ZeroVector;
};
