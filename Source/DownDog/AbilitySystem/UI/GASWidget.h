// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GASWidget.generated.h"

struct FActiveGameplayEffectHandle;
struct FGameplayEffectSpec;
struct FGameplayAttribute;
struct FGameplayTag;
struct FOnAttributeChangeData;
class UAbilitySystemComponent;
/**
 * 
 */
UCLASS()
class DOWNDOG_API UGASWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** Initialize or update references to owner actor and additional actor components (such as AbilitySystemComponent) and cache them for this instance of user widget. */
	UFUNCTION(BlueprintCallable, Category="GAS|UI")
	virtual void SetOwnerActor(AActor* Actor);
	
	/** Returns reference to OwnerActor for this user widget, if it has been initialized. */
	UFUNCTION(BlueprintCallable, Category="GAS|UI")
	virtual AActor* GetOwningActor() const { return OwnerActor; }

	/** Returns reference to AbilitySystemComponent for this user widget, if it has been initialized. */
	UFUNCTION(BlueprintCallable, Category="GAS|UI")
	virtual UAbilitySystemComponent* GetOwningAbilitySystemComponent() const { return AbilitySystemComponent; }
	
	/**
	 * Runs initialization logic for this UserWidget related to interactions with Ability System Component.
	 *
	 * Setup AbilitySystemComponent delegates to react to various events.
	 *
	 * Usually called from NativeConstruct, but exposed there if other classes needs to run initialization logic again to update references.
	 */
	UFUNCTION(BlueprintCallable, Category="GAS|UI")
	virtual void InitializeWithAbilitySystem(UPARAM(ref) const UAbilitySystemComponent* InASC);
    	
protected:
	/** Clears off any ASC delegates and dispose AbilitySystemComponent pointer */
	void ResetAbilitySystem();
	
	/** Register listeners for AbilitySystemComponent (Attributes, GameplayEffects / Tags, Cooldowns, ...) */
	virtual void RegisterAbilitySystemDelegates();

	/** Register listeners for AbilitySystemComponent (Attributes, GameplayEffects / Tags, Cooldowns, ...) */
	virtual void RegisterAttributeDelegates();
	
	/** Returns the current value of an attribute (base value) from owning actor Ability System (if it has any). That is, the value of the attribute with no stateful modifiers */
	UFUNCTION(BlueprintCallable, Category="GAS|UI")
	virtual float GetAttributeValue(FGameplayAttribute Attribute) const;

	/** Helper function to return percentage from Attribute / MaxAttribute */
	UFUNCTION(BlueprintPure, Category="GAS|UI")
	float GetPercentForAttributes(FGameplayAttribute Attribute, FGameplayAttribute MaxAttribute) const;
	
protected: // Event handling
	/**
	 * Event triggered when this widget has been initialized with a valid ASC.
	 * 
	 * Run initialization logic that depends on Ability System here. (like setting initial state for user widgets based on attributes value)
	 */
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|UI")
	void OnAbilitySystemInitialized();
	
	/** Triggered by ASC and handle / broadcast Attributes change */
	virtual void OnAttributeChange(const FOnAttributeChangeData& Data);
	/** Event triggered whenever an attribute value is changed on Owner Actor's ASC */
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|UI", DisplayName="OnAttributeChange")
	void K2_OnAttributeChange(FGameplayAttribute Attribute, float NewValue, float OldValue);
	
	/** Trigger from ASC whenever a gameplay tag is added or removed */
	virtual void OnGameplayTagChange(FGameplayTag GameplayTag, int32 NewTagCount);
	/** Event triggered when a gameplay tag is added or removed to the owner's ASC. Only for 'new' and 'removed' events) */
	UFUNCTION(BlueprintImplementableEvent, Category = "GAS|UI", DisplayName="OnGameplayTagChange")
	void K2_OnGameplayTagChange(FGameplayTag GameplayTag, int32 NewTagCount);

	virtual void NativeDestruct() override;

	void OnGameplayEffectApplied(UAbilitySystemComponent* ASC, const FGameplayEffectSpec& Spec, FActiveGameplayEffectHandle Handle);
	
protected:
	UPROPERTY(BlueprintReadOnly, Category="GAS|UI")
	TObjectPtr<AActor> OwnerActor;
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
