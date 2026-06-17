#include "ProximityVoiceComponent.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/PlayerController.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "TimerManager.h"
#include "Net/VoiceConfig.h"

UProximityVoiceComponent::UProximityVoiceComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UProximityVoiceComponent::BeginPlay()
{
	Super::BeginPlay();

	TryRegisterVOIPTalker();
}

void UProximityVoiceComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	float TargetMouthAmount = 0.0f;

	if (VOIPTalker)
	{
		const float RawVoiceLevel = VOIPTalker->GetVoiceLevel();
		TargetMouthAmount = FMath::Clamp(RawVoiceLevel * VoiceLevelMultiplier, 0.0f, 1.0f);
	}

	MouthOpenAmount = FMath::FInterpTo(
		MouthOpenAmount,
		TargetMouthAmount,
		DeltaTime,
		MouthSmoothingSpeed
	);
}

void UProximityVoiceComponent::TryRegisterVOIPTalker()
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn || !Pawn->GetPlayerState())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &UProximityVoiceComponent::TryRegisterVOIPTalker);
		return;
	}

	VOIPTalker = UVOIPTalker::CreateTalkerForPlayer(Pawn->GetPlayerState());
	if (!VOIPTalker)
	{
		return;
	}

	VOIPTalker->RegisterWithPlayerState(Pawn->GetPlayerState());

	FVoiceSettings Settings;
	Settings.ComponentToAttachTo = Pawn->GetRootComponent();
	Settings.AttenuationSettings = VoiceAttenuation;
	Settings.SourceEffectChain = nullptr;

	VOIPTalker->Settings = Settings;
}

float UProximityVoiceComponent::GetSmoothedVoiceLevel() const
{
	return MouthOpenAmount;
}

void UProximityVoiceComponent::StartTalking()
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn || !Pawn->IsLocallyControlled())
	{
		return;
	}

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (!Subsystem)
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Subsystem->GetVoiceInterface();
	if (!VoiceInterface.IsValid())
	{
		return;
	}

	const int32 LocalUserNum = 0;
	VoiceInterface->StartNetworkedVoice(LocalUserNum);
}

void UProximityVoiceComponent::StopTalking()
{
	APawn* Pawn = Cast<APawn>(GetOwner());
	if (!Pawn || !Pawn->IsLocallyControlled())
	{
		return;
	}

	IOnlineSubsystem* Subsystem = Online::GetSubsystem(GetWorld());
	if (!Subsystem)
	{
		return;
	}

	IOnlineVoicePtr VoiceInterface = Subsystem->GetVoiceInterface();
	if (!VoiceInterface.IsValid())
	{
		return;
	}

	const int32 LocalUserNum = 0;
	VoiceInterface->StopNetworkedVoice(LocalUserNum);
}