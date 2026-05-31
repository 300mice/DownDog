// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Attributes/DownDogPlayerSet.h"

#include "Net/UnrealNetwork.h"

UDownDogPlayerSet::UDownDogPlayerSet()
	: Health(100.0f)
	, MaxHealth(100.0f)
	, MovementSpeed( 600.0f)
	, Stamina(100)
	, MaxStamina(100)
	, StaminaRegeneration(1)
{
	bOutOfHealth = false;
	MaxHealthBeforeAttributeChange = 0.0f;
	HealthBeforeAttributeChange = 0.0f;
}

void UDownDogPlayerSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UDownDogPlayerSet, Health, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDownDogPlayerSet, MaxHealth, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UDownDogPlayerSet, MovementSpeed, COND_None, REPNOTIFY_Always);
}

void UDownDogPlayerSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDownDogPlayerSet, Health, OldValue);

	// Call the change callback, but without an instigator
	// This could be changed to an explicit RPC in the future
	// These events on the client should not be changing attributes

	const float CurrentHealth = GetHealth();
	const float EstimatedMagnitude = CurrentHealth - OldValue.GetCurrentValue();
	
	OnHealthChanged.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);

	if (!bOutOfHealth && CurrentHealth <= 0.0f)
	{
		OnOutOfHealth.Broadcast(nullptr, nullptr, nullptr, EstimatedMagnitude, OldValue.GetCurrentValue(), CurrentHealth);
	}

	bOutOfHealth = (CurrentHealth <= 0.0f);
}

void UDownDogPlayerSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDownDogPlayerSet, MaxHealth, OldValue);
}

void UDownDogPlayerSet::OnRep_MovementSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDownDogPlayerSet, MovementSpeed, OldValue);
}

void UDownDogPlayerSet::OnRep_Stamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDownDogPlayerSet, Stamina, OldValue);
}

void UDownDogPlayerSet::OnRep_MaxStamina(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDownDogPlayerSet, MaxStamina, OldValue);
}

void UDownDogPlayerSet::OnRep_StaminaRegeneration(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UDownDogPlayerSet, StaminaRegeneration, OldValue);
}

bool UDownDogPlayerSet::PreGameplayEffectExecute(FGameplayEffectModCallbackData& Data)
{
	HealthBeforeAttributeChange = GetHealth();
	MaxHealthBeforeAttributeChange = GetMaxHealth();
	return Super::PreGameplayEffectExecute(Data);
}

void UDownDogPlayerSet::PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const
{
	Super::PreAttributeBaseChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UDownDogPlayerSet::PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	ClampAttribute(Attribute, NewValue);
}

void UDownDogPlayerSet::PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue)
{
	Super::PostAttributeChange(Attribute, OldValue, NewValue);

	if (Attribute == GetMaxHealthAttribute())
	{
		UAbilitySystemComponent* LyraASC = GetAbilitySystemComponent();
		check(LyraASC);

		//When we gain max health, heal for that amount as well 
		if (GetHealth() < NewValue)
		{
			LyraASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Additive, NewValue - OldValue);
		}

		// Make sure current health is not greater than the new max health.
		if (GetHealth() > NewValue)
		{
			LyraASC->ApplyModToAttribute(GetHealthAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (Attribute == GetMaxStaminaAttribute())
	{
		UAbilitySystemComponent* LyraASC = GetAbilitySystemComponent();
		check(LyraASC);

		//When we gain max health, heal for that amount as well 
		if (GetStamina() < NewValue)
		{
			LyraASC->ApplyModToAttribute(GetStaminaAttribute(), EGameplayModOp::Additive, NewValue - OldValue);
		}

		// Make sure current health is not greater than the new max health.
		if (GetStamina() > NewValue)
		{
			LyraASC->ApplyModToAttribute(GetStaminaAttribute(), EGameplayModOp::Override, NewValue);
		}
	}

	if (bOutOfHealth && (GetHealth() > 0.0f))
	{
		bOutOfHealth = false;
	}
}

void UDownDogPlayerSet::ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const
{
	if (Attribute == GetHealthAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxHealthAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetMovementSpeedAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.0f);
	}
	else if (Attribute == GetStaminaAttribute())
	{
		// Do not allow health to go negative or above max health.
		NewValue = FMath::Clamp(NewValue, 0.0f, GetMaxHealth());
	}
	else if (Attribute == GetMaxStaminaAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 1.0f);
	}
	else if (Attribute == GetStaminaRegenerationAttribute())
	{
		// Do not allow max health to drop below 1.
		NewValue = FMath::Max(NewValue, 0.0f);
	}
}

