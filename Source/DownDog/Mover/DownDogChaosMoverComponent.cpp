// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover/DownDogChaosMoverComponent.h"

#include "ChaosMover/Backends/ChaosMoverBackend.h"
#include "ChaosMover/Character/ChaosCharacterInputs.h"
#include "DefaultMovementSet/CharacterMoverSimulationTypes.h"
#include "GameFramework/Pawn.h"

void UDownDogChaosMoverComponent::AddExternalForce(FVector Force)
{
	AccumulatedExternalForce += Force;
}

void UDownDogChaosMoverComponent::ProduceInput(const int32 DeltaTimeMS, FMoverInputCmdContext* Cmd)
{
	Super::ProduceInput(DeltaTimeMS, Cmd);

	if (!AccumulatedExternalForce.IsZero())
	{
		float DeltaTimeSeconds = DeltaTimeMS / 1000.f;
		FChaosMoverLaunchInputs& LaunchInputs = 
			Cmd->InputCollection.FindOrAddMutableDataByType<FChaosMoverLaunchInputs>();
        
		// Add to any existing launch (e.g. a real jump happening same frame)
		LaunchInputs.LaunchVelocityOrImpulse += AccumulatedExternalForce * DeltaTimeSeconds;
		LaunchInputs.Mode = EChaosMoverVelocityEffectMode::AdditiveVelocity;

		AccumulatedExternalForce = FVector::ZeroVector; // Clear each frame
	}
	
}
