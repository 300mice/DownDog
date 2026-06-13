// Copyright Epic Games, Inc. All Rights Reserved.


#include "DownDogPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "DownDogCameraManager.h"
#include "Blueprint/UserWidget.h"
#include "DownDog.h"
#include "DownDogCharacter.h"
#include "Input/ModularInputComponent.h"
#include "Widgets/Input/SVirtualJoystick.h"

ADownDogPlayerController::ADownDogPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = ADownDogCameraManager::StaticClass();
}

void ADownDogPlayerController::BeginPlay()
{
	Super::BeginPlay();
	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogDownDog, Error, TEXT("Could not spawn mobile controls widget."));

		}
	}
}

void ADownDogPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}

		if (InputConfig)
		{
			UModularInputComponent* ModularInputComponent = Cast<UModularInputComponent>(InputComponent);
			check(ModularInputComponent);

			for (FTaggedInputAction& TaggedAction : InputConfig->TaggedInputActions)
			{
				// Bind press
				ModularInputComponent->BindActionByTag(
					InputConfig, TaggedAction.InputTag,
					ETriggerEvent::Started,
					this, &ADownDogPlayerController::Input_AbilityInputTagPressed,
					TaggedAction.InputTag);

				// Bind release
				ModularInputComponent->BindActionByTag(
					InputConfig, TaggedAction.InputTag,
					ETriggerEvent::Completed,
					this, &ADownDogPlayerController::Input_AbilityInputTagReleased,
					TaggedAction.InputTag);
			}
		}
	}
	
}

bool ADownDogPlayerController::ShouldUseTouchControls() const
{
	// are we on a mobile platform? Should we force touch?
	return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
}

void ADownDogPlayerController::Input_AbilityInputTagPressed(FGameplayTag InputTag)
{
	ADownDogCharacter* DownDogCharacter = Cast<ADownDogCharacter>(GetPawn());
	if (!DownDogCharacter || !DownDogCharacter->ASC) return;

	TArray<FGameplayAbilitySpecHandle> Handles;
	DownDogCharacter->ASC->GetAllAbilities(Handles);

	for (FGameplayAbilitySpecHandle& Handle : Handles)
	{
		FGameplayAbilitySpec* Spec = DownDogCharacter->ASC->FindAbilitySpecFromHandle(Handle);
		if (!Spec) continue;

		if (Spec->GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			// This sets Spec->InputPressed = true AND fires InputPressed replication event
			DownDogCharacter->ASC->AbilityLocalInputPressed(Spec->InputID);
		}
	}
}

void ADownDogPlayerController::Input_AbilityInputTagReleased(FGameplayTag InputTag)
{
	ADownDogCharacter* DownDogCharacter = Cast<ADownDogCharacter>(GetPawn());
	if (!DownDogCharacter || !DownDogCharacter->ASC) return;

	TArray<FGameplayAbilitySpecHandle> Handles;
	DownDogCharacter->ASC->GetAllAbilities(Handles);

	for (FGameplayAbilitySpecHandle& Handle : Handles)
	{
		FGameplayAbilitySpec* Spec = DownDogCharacter->ASC->FindAbilitySpecFromHandle(Handle);
		if (!Spec) continue;

		if (Spec->GetDynamicSpecSourceTags().HasTag(InputTag))
		{
			// This sets Spec->InputPressed = false AND fires InputReleased replication event
			DownDogCharacter->ASC->AbilityLocalInputReleased(Spec->InputID);
		}
	}
}

void ADownDogPlayerController::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	if (IsLocalPlayerController())
	{
		StartTalking();
	}
}
