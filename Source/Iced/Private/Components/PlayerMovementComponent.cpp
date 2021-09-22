// Narckomanser's game


#include "Components/PlayerMovementComponent.h"

#include "BasePlayer.h"

float UPlayerMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();

	const auto Player = Cast<ABasePlayer>(GetCharacterOwner());
	if (!Player) { return 0; }

	return Player->IsRunning() ? MaxSpeed * RunSpeedModifier : MaxSpeed;
}
