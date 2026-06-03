// Fill out your copyright notice in the Description page of Project Settings.


#include "DownDogItemSet.h"

#include "Net/UnrealNetwork.h"

UDownDogItemSet::UDownDogItemSet()
	: Weight(50.0f)
{
}

void UDownDogItemSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDownDogItemSet, Weight, COND_None, REPNOTIFY_Always);
}

void UDownDogItemSet::OnRep_Weight(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDownDogItemSet, Weight, OldValue);
}
