// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AbilitySystem/Attributes/DownDogAttributeSet.h"
#include "DownDogPlayerSet.generated.h"

/**
 * 
 */
UCLASS()
class DOWNDOG_API UDownDogPlayerSet : public UDownDogAttributeSet
{
	GENERATED_BODY()

public:
	UDownDogPlayerSet();
		

	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, Health);
	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, MaxHealth);
	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, MovementSpeed);
	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, Stamina);
	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, MaxStamina);
	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, StaminaRegeneration);
	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, CarryingCapacity);
	ATTRIBUTE_ACCESSORS(UDownDogPlayerSet, CarryHeight);

	// Delegate when health changes due to damage/healing, some information may be missing on the client
	mutable FDownDogAttributeEvent OnHealthChanged;

	// Delegate when max health changes
	mutable FDownDogAttributeEvent OnMaxHealthChanged;

	// Delegate to broadcast when the health attribute reaches zero
	mutable FDownDogAttributeEvent OnOutOfHealth;

	// Delegate when max health changes
	mutable FDownDogAttributeEvent OnMovementSPeedChanged;

	mutable FDownDogAttributeEvent OnStaminaChanged;
	mutable FDownDogAttributeEvent OnMaxStaminaChanged;
	mutable FDownDogAttributeEvent OnStaminaRegenerationChanged;

	mutable FDownDogAttributeEvent OnCarryingCapacityChanged;



protected:
	//Health Rep
	UFUNCTION()
	void OnRep_Health(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxHealth(const FGameplayAttributeData& OldValue);

	//Movement Rep
	UFUNCTION()
	void OnRep_MovementSpeed(const FGameplayAttributeData& OldValue);

	//Stamina Rep
	UFUNCTION()
	void OnRep_Stamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_MaxStamina(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_StaminaRegeneration(const FGameplayAttributeData& OldValue);

	//Capacity Rep
	UFUNCTION()
	void OnRep_CarryingCapacity(const FGameplayAttributeData& OldValue);
	UFUNCTION()
	void OnRep_CarryHeight(const FGameplayAttributeData& OldValue);
	

	virtual bool PreGameplayEffectExecute(struct FGameplayEffectModCallbackData& Data) override;

	virtual void PreAttributeBaseChange(const FGameplayAttribute& Attribute, float& NewValue) const override;
	virtual void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	virtual void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

	void ClampAttribute(const FGameplayAttribute& Attribute, float& NewValue) const;
	
private:
	// The current health attribute.  The health will be capped by the max health attribute.  Health is hidden from modifiers so only executions can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Health, Category = "DownDog|Health", Meta = (HideFromModifiers, AllowPrivateAccess = true))
	FGameplayAttributeData Health;

	// The current max health attribute.  Max health is an attribute since gameplay effects can modify it.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHealth, Category = "DownDog|Health", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxHealth;

	// The current movement speed attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MovementSpeed, Category = "DownDog|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MovementSpeed;

	// The current stamina attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Stamina, Category = "DownDog|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData Stamina;

	// The maximum stamina attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxStamina, Category = "DownDog|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData MaxStamina;

	// The rate at which stamina regenerates attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_StaminaRegeneration, Category = "DownDog|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData StaminaRegeneration;

	// The rate at which stamina regenerates attribute.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CarryingCapacity, Category = "DownDog|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CarryingCapacity;

	// The height at which you carry objects.
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_CarryHeight, Category = "DownDog|Movement", Meta = (AllowPrivateAccess = true))
	FGameplayAttributeData CarryHeight;

	// Used to track when the health reaches 0.
	bool bOutOfHealth;

	// Store the health before any changes 
	float MaxHealthBeforeAttributeChange;
	float HealthBeforeAttributeChange;

	// Incoming damage. This is mapped directly to -Health
	UPROPERTY(BlueprintReadOnly, Category="Lyra|Health", Meta=(HideFromModifiers, AllowPrivateAccess=true))
	FGameplayAttributeData Damage;
};
