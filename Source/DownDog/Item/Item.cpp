// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "AbilitySystemComponent.h"
#include "AnchorLocation.h"
#include "HandleLocation.h"
#include "AbilitySystem/Attributes/DownDogItemSet.h"

// Sets default values
AItem::AItem()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);
	ItemMesh->SetSimulatePhysics(true);
	ItemMesh->SetAngularDamping(2.5f);
	ItemMesh->SetLinearDamping(0.4f);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	
	bReplicates = true;
	this->AActor::SetReplicateMovement(true);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	ASC->InitAbilityActorInfo(this, this);
	const UDownDogItemSet* ItemAttributeSet = Cast<UDownDogItemSet>(ASC->GetAttributeSet(UDownDogItemSet::StaticClass()));
	if (ItemAttributeSet)
	{
		ItemMesh->SetMassOverrideInKg("NAME_None", ItemAttributeSet->GetWeight());
	}
}

UAnchorLocation* AItem::GetClosestAnchor(FVector InLocation)
{
	TArray<TObjectPtr<UAnchorLocation>> AnchorLocations;
	GetComponents<UAnchorLocation>(AnchorLocations);
	UAnchorLocation* OutAnchor = nullptr;
	float MinDistance = 100000;

	for (UAnchorLocation* Anchor : AnchorLocations)
	{
		FVector HandleLocation = Anchor->GetComponentLocation();
		if (float Distance = (InLocation - HandleLocation).Size(); Distance < MinDistance)
		{
			MinDistance = Distance;
			OutAnchor = Anchor;
		}
	}
	return OutAnchor;
}

FVector AItem::GetClosestAnchorLocation(FVector InLocation)
{
	TArray<TObjectPtr<UAnchorLocation>> AnchorLocations;
	GetComponents<UAnchorLocation>(AnchorLocations);
	UAnchorLocation* OutAnchor = nullptr;
	float MinDistance = 100000;

	for (UAnchorLocation* Anchor : AnchorLocations)
	{
		FVector HandleLocation = Anchor->GetComponentLocation();
		if (float Distance = (InLocation - HandleLocation).Size(); Distance < MinDistance)
		{
			MinDistance = Distance;
			OutAnchor = Anchor;
		}
	}
	return OutAnchor->GetComponentLocation();
}

USceneComponent* AItem::GetClosestHandle(FVector InLocation)
{
	UAnchorLocation* Anchor = GetClosestAnchor(InLocation);
	TArray<USceneComponent*> Handles;
	Anchor->GetChildrenComponents(false, Handles);
	USceneComponent* OutHandle = nullptr;
	float MinDistance = 100000;

	for (USceneComponent* Handle : Handles)
	{
		FVector HandleLocation = Handle->GetComponentLocation();
		if (float Distance = (Handle->GetComponentLocation() - InLocation).Size(); Distance < MinDistance)
		{
			MinDistance = Distance;
			OutHandle = Handle;
		}
	}
	return OutHandle;
}



