// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UAbilitySystemComponent;

UCLASS()
class DOWNDOG_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	//Components
protected:

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category="Components")
	UAbilitySystemComponent* ASC;

	UFUNCTION(BlueprintCallable, Category = "Item")
	FVector GetClosestHandle(FVector InLocation);

};
