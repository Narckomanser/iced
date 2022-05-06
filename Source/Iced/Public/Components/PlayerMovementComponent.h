// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Iced/Public/CoreTypes.h"
#include "PlayerMovementComponent.generated.h"

UCLASS()
class ICED_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual float GetMaxSpeed() const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FOnExperienceUp OnExperienceUp;

protected:
	virtual void BeginPlay() override;

private:
	void CalculateTravelledDistance(const float DeltaTime);

private:
	UPROPERTY(EditDefaultsOnly)
	float RunSpeedModifier = 2.f; // TODO Probably other modifiers related to the state of the character will be used

	UPROPERTY(EditDefaultsOnly, Category = "Experience")
	float DistanceUpdateValue = 0.f;

	float TotalDistanceTravelled = 0.f;
};
