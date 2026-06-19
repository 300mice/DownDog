// Fill out your copyright notice in the Description page of Project Settings.


#include "Mover/Modes/DownDogPullingMode.h"

#include "DownDogCharacter.h"
#include "Item/AnchorLocation.h"

class UAnchorLocation;

float UDownDogPullingMode::GetMaxSpeed() const
{
	if (!DownDogCharacter || !HeldItem)
	{
		return Super::GetMaxSpeed();
	}

	const FVector ObjectToPlayerDir =
		(DownDogCharacter->GetActorLocation() - HeldItem->GetActorLocation()).GetSafeNormal();
	const FVector PlayerMoveDir =
	DownDogCharacter->GetVelocity().GetSafeNormal();
	const float MovingAwayDot = FVector::DotProduct(PlayerMoveDir, ObjectToPlayerDir);
	const bool bMovingAwayFromObject = MovingAwayDot > 0.f;
	
	if (bMovingAwayFromObject)
	{
		HeldItem->GetMesh()->AddForceAtLocation((PullForce * HeldItem->GetMesh()->GetMass()) * ObjectToPlayerDir, ClosestAnchor->GetComponentLocation());
		float NewMaxSpeed = Super::GetMaxSpeed();
		float DistanceFromHeldObject = (DownDogCharacter->GetActorLocation() - ClosestAnchor->GetComponentLocation()).Size();
		float NormalizedDistance = 1 - FMath::GetMappedRangeValueClamped(FVector2D(0, MaximumDistanceFromObject), FVector2D(0,1), DistanceFromHeldObject);
		
		return NewMaxSpeed * NormalizedDistance;
	}

	return Super::GetMaxSpeed();
	
}

void UDownDogPullingMode::Activate()
{
	Super::Activate();

	DownDogCharacter = Cast<ADownDogCharacter>(GetMoverComponent()->GetOwner());
	if (DownDogCharacter)
	{
		HeldItem = DownDogCharacter->GetHeldItem();
		if (HeldItem)
		{
			ClosestAnchor = HeldItem->GetClosestAnchor(DownDogCharacter->GetActorLocation());
		}
	}
}
