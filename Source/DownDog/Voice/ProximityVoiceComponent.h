#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProximityVoiceComponent.generated.h"

class UVOIPTalker;

UCLASS(ClassGroup=(Voice), meta=(BlueprintSpawnableComponent))
class UProximityVoiceComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProximityVoiceComponent();

	virtual void BeginPlay() override;

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void StartTalking();

	UFUNCTION(BlueprintCallable)
	void StopTalking();

protected:
	UPROPERTY()
	TObjectPtr<UVOIPTalker> VOIPTalker;

	UPROPERTY(EditDefaultsOnly, Category="Voice")
	TObjectPtr<USoundAttenuation> VoiceAttenuation;

	UPROPERTY(EditDefaultsOnly, Category="Voice")
	FVector MouthOffset = FVector(0.f, 0.f, 70.f);

	UPROPERTY(EditDefaultsOnly, Category="Voice")
	float VoiceVolume = 1.0f;

	void TryRegisterVOIPTalker();

public:
	UFUNCTION(BlueprintCallable, Category="Voice")
	float GetSmoothedVoiceLevel() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voice|Mouth")
	float MouthOpenAmount = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voice|Mouth")
	float VoiceLevelMultiplier = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Voice|Mouth")
	float MouthSmoothingSpeed = 12.0f;
};