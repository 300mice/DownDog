// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/DownDogGameplayAbility.h"

ADownDogCharacter* UDownDogGameplayAbility::GetDownDogCharacterFromActorInfo()
{
	return Cast<ADownDogCharacter>(GetActorInfo().AvatarActor);
}
