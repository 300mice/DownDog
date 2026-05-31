// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/UI/GASWidget.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemGlobals.h"

void UGASWidget::SetOwnerActor(AActor* Actor)
{
	OwnerActor = Actor;
	AbilitySystemComponent = UAbilitySystemGlobals::GetAbilitySystemComponentFromActor(Actor);

	InitializeWithAbilitySystem(AbilitySystemComponent);
}

void UGASWidget::InitializeWithAbilitySystem(const UAbilitySystemComponent* InASC)
{
	if (!InASC)
	{
		return;
	}
	
	if (!OwnerActor)
	{
		SetOwnerActor(InASC->GetOwnerActor());
	}

	if (AbilitySystemComponent && AbilitySystemComponent != InASC)
	{
		ResetAbilitySystem();
	}

	AbilitySystemComponent = const_cast<UAbilitySystemComponent*>(InASC);
	RegisterAbilitySystemDelegates();

	// Broadcast info to Blueprints
	OnAbilitySystemInitialized();
}

void UGASWidget::ResetAbilitySystem()
{
	AbilitySystemComponent = nullptr;
}

void UGASWidget::RegisterAbilitySystemDelegates()
{
	if (!AbilitySystemComponent) return;

	// Register current attributes
	RegisterAttributeDelegates();

	// Listen for new GEs being applied (which may grant new attribute sets)
	AbilitySystemComponent->OnGameplayEffectAppliedDelegateToSelf.AddUObject(
		this, &UGASWidget::OnGameplayEffectApplied);
}

void UGASWidget::RegisterAttributeDelegates()
{
	if (!AbilitySystemComponent) return;

	TArray<FGameplayAttribute> Attributes;
	AbilitySystemComponent->GetAllAttributes(Attributes);

	for (FGameplayAttribute Attribute : Attributes)
	{
		FOnGameplayAttributeValueChange& Delegate = 
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute);

		// Only bind if not already bound to avoid duplicate callbacks
		if (!Delegate.IsBoundToObject(this))
		{
			Delegate.AddUObject(this, &UGASWidget::OnAttributeChange);
		}
	}
}

void UGASWidget::OnGameplayEffectApplied(
	UAbilitySystemComponent* ASC,
	const FGameplayEffectSpec& Spec,
	FActiveGameplayEffectHandle Handle)
{
	// Re-register all attributes - will pick up any newly added attribute sets
	RegisterAttributeDelegates();
}

float UGASWidget::GetAttributeValue(FGameplayAttribute Attribute) const
{
	if (!AbilitySystemComponent)
	{
		return 0.0f;
	}

	if (!AbilitySystemComponent->HasAttributeSetForAttribute(Attribute))
	{
		return 0.0f;
	}

	return AbilitySystemComponent->GetNumericAttribute(Attribute);
}

float UGASWidget::GetPercentForAttributes(FGameplayAttribute Attribute, FGameplayAttribute MaxAttribute) const
{
	const float AttributeValue = GetAttributeValue(Attribute);
	const float MaxAttributeValue = GetAttributeValue(MaxAttribute);

	if (MaxAttributeValue == 0.f)
	{
		return 0.f;
	}

	return AttributeValue / MaxAttributeValue;
}

void UGASWidget::OnAttributeChange(const FOnAttributeChangeData& Data)
{
	// Broadcast event to Blueprint
	K2_OnAttributeChange(Data.Attribute, Data.NewValue, Data.OldValue);
}

void UGASWidget::OnGameplayTagChange(FGameplayTag GameplayTag, int32 NewTagCount)
{
	K2_OnGameplayTagChange(GameplayTag, NewTagCount);
}

void UGASWidget::NativeDestruct()
{
	if (AbilitySystemComponent)
	{
		TArray<FGameplayAttribute> Attributes;
		AbilitySystemComponent->GetAllAttributes(Attributes);
		for (FGameplayAttribute Attribute : Attributes)
		{
			AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(Attribute)
				.RemoveAll(this);
		}
		ResetAbilitySystem();
	}

	Super::NativeDestruct();
}
