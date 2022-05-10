// Narckomanser's game


#include "Components/PlayerMovementComponent.h"

#include "BasePlayer.h"
#include "ProgressComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogPlayerMovementComponent, All, All);

float UPlayerMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();

	const auto Player = Cast<ABasePlayer>(GetCharacterOwner());
	if (!Player) { return 0; }

	return Player->IsRunning() ? MaxSpeed * RunSpeedModifier : MaxSpeed;
}

void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                             FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	CalculateTravelledDistance(DeltaTime);
}

void UPlayerMovementComponent::CalculateTravelledDistance(const float DeltaTime)
{
	TotalDistanceTravelled += Velocity.Length() * DeltaTime;

	if (const auto Owner = GetOwner<ABasePlayer>(); Owner && TotalDistanceTravelled >= DistanceUpdateValue)
	{
		UE_LOG(LogPlayerMovementComponent, Display, TEXT("TotalDistanceTravelled: %f by : %s"), TotalDistanceTravelled,
		       *GetOwner()->GetName());

		Owner->OnExperienceUp.Broadcast(EAttributes::Constitution, GetOwner());
		TotalDistanceTravelled = 0.f;
	}
}
