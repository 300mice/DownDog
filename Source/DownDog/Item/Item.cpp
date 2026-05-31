// Fill out your copyright notice in the Description page of Project Settings.


#include "Item.h"

#include "AbilitySystemComponent.h"
#include "HandleLocation.h"

// Sets default values
AItem::AItem()
{
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Item Mesh"));
	SetRootComponent(ItemMesh);
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ASC = CreateDefaultSubobject<UAbilitySystemComponent>("ASC");
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	ASC->InitAbilityActorInfo(this, this);
}

FVector AItem::GetClosestHandle(FVector InLocation)
{
	TArray<TObjectPtr<UHandleLocation>> HandleLocations;
	GetComponents<UHandleLocation>(HandleLocations);
	FVector OutLocation = FVector::ZeroVector;
	float MinDistance = 10000;

	for (const UHandleLocation* Handle : HandleLocations)
	{
		FVector HandleLocation = Handle->GetComponentLocation();
		if (float Distance = (InLocation - HandleLocation).Size(); Distance < MinDistance)
		{
			MinDistance = Distance;
			OutLocation = HandleLocation;
		}
	}
	return OutLocation;
}

