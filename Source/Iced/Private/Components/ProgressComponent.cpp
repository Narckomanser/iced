// Narckomanser's game


#include "ProgressComponent.h"

#include "BasePlayer.h"


UProgressComponent::UProgressComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProgressComponent::BeginPlay()
{
	Super::BeginPlay();

	if(const auto Owner = GetOwner<ABasePlayer>(); Owner)
	{
		if (const auto MovementComponent = Cast<UPlayerMovementComponent>(Owner->GetMovementComponent()); MovementComponent)
		{
			MovementComponent->OnExperienceUp.AddUObject(this, &UProgressComponent::FOnExperienceUpHandler);
		}
	}
}

void UProgressComponent::FOnExperienceUpHandler(EAttributes Attribute, AActor* AttributeOwner)
{
	if (AttributeOwner == GetOwner())
	{
		auto [AttributeLevel, AttributeExperience, ExpToLvlUp] = AttributesLevel[Attribute];
		AttributeExperience++;

		if (AttributeExperience == ExpToLvlUp[AttributeLevel])
		{
			AttributeLevel++;
		}
	}
	
}

void UProgressComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
