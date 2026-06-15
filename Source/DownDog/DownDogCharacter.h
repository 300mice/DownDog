// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "MoverSimulationTypes.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "DefaultMovementSet/NavMoverComponent.h"
#include "GameFramework/Pawn.h"
#include "Logging/LogMacros.h"
#include "DownDogCharacter.generated.h"

struct FAbilitySet_GrantedHandles;
class UAbilitySet;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class ADownDogCharacter : public APawn, public IMoverInputProducerInterface
{
	GENERATED_BODY()

protected:
	
	/** Pawn Capsule */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;

	/** Pawn mesh: third person view */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* ThirdPersonMesh;

	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(Category = Movement, VisibleAnywhere, BlueprintReadOnly, Transient, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCharacterMoverComponent> CharacterMotionComponent;

	/** Holds functionality for nav movement data and functions */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Transient, Category="Nav Movement")
	TObjectPtr<UNavMoverComponent> NavMoverComponent;

	//MOVEMENT
protected:

	// Entry point for input production. Do not override. To extend in derived character types, override OnProduceInput for native types or implement "Produce Input" blueprint event
	virtual void ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult) override;

	// Override this function in native class to author input for the next simulation frame. Consider also calling Super method.
	virtual void OnProduceInput(float DeltaMs, FMoverInputCmdContext& InputCmdResult);

	// Implement this event in Blueprints to author input for the next simulation frame. Consider also calling Parent event.
	UFUNCTION(BlueprintImplementableEvent, DisplayName="On Produce Input", meta = (ScriptName = "OnProduceInput"))
	FMoverInputCmdContext OnProduceInputInBlueprint(float DeltaMs, FMoverInputCmdContext InputCmd);

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category ="Input")
	class UInputAction* MouseLookAction;
	
public:

	ADownDogCharacter(const class FObjectInitializer& ObjectInitializer);

	// Accessor for the actor's movement component
	UFUNCTION(BlueprintPure, Category = Mover)
	UCharacterMoverComponent* GetMoverComponent() const { return CharacterMotionComponent; }

	// Request the character starts moving with an intended directional magnitude. A length of 1 indicates maximum acceleration.
	UFUNCTION(BlueprintCallable, Category=MoverExamples)
	virtual void RequestMoveByIntent(const FVector& DesiredIntent) { CachedMoveInputIntent = DesiredIntent; }

	// Request the character starts moving with a desired velocity. This will be used in lieu of any other input.
	UFUNCTION(BlueprintCallable, Category=MoverExamples)
	virtual void RequestMoveByVelocity(const FVector& DesiredVelocity) { CachedMoveInputVelocity=DesiredVelocity; }

	//~ Begin INavAgentInterface Interface
	virtual FVector GetNavAgentLocation() const override;
	//~ End INavAgentInterface Interface
	
	virtual void UpdateNavigationRelevance() override;

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:
	
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

protected:

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	

public:

	// Whether or not we author our movement inputs relative to whatever base we're standing on, or leave them in world space. Only applies if standing on a base of some sort.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MoverExamples)
	bool bUseBaseRelativeMovement = true;
	
	/**
	 * If true, rotate the Character toward the direction the actor is moving
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=MoverExamples)
	bool bOrientRotationToMovement = true;
	
	/**
	 * If true, the actor will continue orienting towards the last intended orientation (from input) even after movement intent input has ceased.
	 * This makes the character finish orienting after a quick stick flick from the player.  If false, character will not turn without input.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = MoverExamples)
	bool bMaintainLastInputOrientation = false;

	// Extra Components
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Components")
	UAbilitySystemComponent* ASC;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Abilities")
	TObjectPtr<UAbilitySet> StartingAbilities;

	FAbilitySet_GrantedHandles* AbilityHandles;

private:

	FVector LastAffirmativeMoveInput = FVector::ZeroVector;	// Movement input (intent or velocity) the last time we had one that wasn't zero

	FVector CachedMoveInputIntent = FVector::ZeroVector;
	FVector CachedMoveInputVelocity = FVector::ZeroVector;

	FRotator CachedTurnInput = FRotator::ZeroRotator;
	FRotator CachedLookInput = FRotator::ZeroRotator;

	bool bIsJumpJustPressed = false;
	bool bIsJumpPressed = false;
	bool bIsFlyingActive = false;
	bool bShouldToggleFlying = false;

	void OnMoveTriggered(const FInputActionValue& Value);
	void OnMoveCompleted(const FInputActionValue& Value);
	void OnJumpStarted(const FInputActionValue& Value);
	void OnJumpReleased(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	uint8 bHasProduceInputinBpFunc : 1;

	UFUNCTION(Server,UnReliable)
	void ServerUpdateControlRotation(FRotator NewRotation);
	

};

