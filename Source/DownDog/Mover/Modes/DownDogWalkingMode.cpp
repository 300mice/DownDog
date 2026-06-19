// Fill out your copyright notice in the Description page of Project Settings.


#include "DownDogWalkingMode.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Mover/DownDogChaosMoverComponent.h"
#include "AbilitySystem/Attributes/DownDogPlayerSet.h"

class UDownDogPlayerSet;
class UAbilitySystemComponent;

float UDownDogWalkingMode::GetMaxSpeed() const
{
	UAbilitySystemComponent* ASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetMoverComponent()->GetOwner());
	if (!ASC) return Super::GetMaxSpeed();
	const UDownDogPlayerSet* PlayerAttributeSet = Cast<UDownDogPlayerSet>(ASC->GetAttributeSet(UDownDogPlayerSet::StaticClass()));
	if (!PlayerAttributeSet) return Super::GetMaxSpeed();

	return PlayerAttributeSet->GetMovementSpeed();
}
