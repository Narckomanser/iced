// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerMovementComponent.generated.h"

UCLASS()
class ICED_API UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual float GetMaxSpeed() const override;

private:
	const float RunSpeedModifier = 2.f; // Probably other modifiers related to the state of the character will be used
};
