// Narckomanser's game


#include "ProgressComponent.h"

#include "BasePlayer.h"

DEFINE_LOG_CATEGORY_STATIC(LogProgressComponent, All, All);

UProgressComponent::UProgressComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProgressComponent::BeginPlay()
{
	Super::BeginPlay();

	if (const auto Owner = GetOwner<ABasePlayer>(); Owner)
	{
		Owner->OnExperienceUp.AddUObject(this, &UProgressComponent::FOnExperienceUpHandler);
	}
}

void UProgressComponent::FOnExperienceUpHandler(EAttributes Attribute, AActor* AttributeOwner)
{
	if (AttributeOwner == GetOwner())
	{
		auto& [AttributeLevel, AttributeExperience, ExpToLvlUp] = AttributesData[Attribute];

		if (AttributeLevel != ExpToLvlUp.Num())
		{
			++AttributeExperience;
			
			if (AttributeExperience == ExpToLvlUp[AttributeLevel])
			{
				AttributeLevel++;
			}
		}
	}

	UE_LOG(LogProgressComponent, Display, TEXT("LevelInfo: %s"), *AttributesData[Attribute].ToString());
}


void UProgressComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
