// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DownDogAttributeSet.h"
#include "DownDogItemSet.generated.h"

/**
 * 
 */
UCLASS()
class DOWNDOG_API UDownDogItemSet : public UDownDogAttributeSet
{
	GENERATED_BODY()

public:

	UDownDogItemSet();

	ATTRIBUTE_ACCESSORS(UDownDogItemSet, Weight);
	mutable FDownDogAttributeEvent OnWeightChanged;
protected:

	UFUNCTION()
	void OnRep_Weight(const FGameplayAttributeData& OldValue);

private:
	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Weight, Category = "DownDog|Item", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Weight;
};
