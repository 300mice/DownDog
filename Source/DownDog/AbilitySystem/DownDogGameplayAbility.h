// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DownDogCharacter.h"
#include "Abilities/GameplayAbility.h"
#include "DownDogGameplayAbility.generated.h"

/**
 * 
 */
UCLASS(ClassGroup = (GameplayAbility))
class DOWNDOG_API UDownDogGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Ability")
	ADownDogCharacter* GetDownDogCharacterFromActorInfo();
};
