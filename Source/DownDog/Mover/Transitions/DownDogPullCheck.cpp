// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover/Transitions/DownDogPullCheck.h"

#include "MoverComponent.h"

UE_DEFINE_GAMEPLAY_TAG(TAG_EnhancedInput_Action_Pull, "EnhancedInput.Action.Pull");

FTransitionEvalResult UDownDogPullCheck::Evaluate_Implementation(const FSimulationTickParams& Params) const
{
	FTransitionEvalResult EvalResult;
	if (GetMoverComponent()->HasGameplayTag(TAG_EnhancedInput_Action_Pull, true))
	{
		EvalResult.NextMode = PullingModeName;
	}
	else
	{
		EvalResult.NextMode = WalkingModeName;
	}
	
	return EvalResult;
	
}
