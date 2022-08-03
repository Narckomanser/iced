// Narckomanser's game


#include "ProgressComponent.h"

#include "BasePlayer.h"
#include "Iced/Public/CoreTypes.h"
#include "BasePlayerState.h"

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
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto PlayerState = Cast<ABasePlayerState>(Owner->GetPlayerState());
	if (!PlayerState) { return; }
	
	if (AttributeOwner == Owner)
	{
		if (auto& [AttributeLevel, AttributeExperience, ExpToLvlUp] = PlayerState->GetLevelData(Attribute);
			AttributeLevel != ExpToLvlUp.Num())
		{
			AttributeExperience++;

			if (AttributeExperience == ExpToLvlUp[AttributeLevel])
			{
				AttributeLevel++;
			}
		}

		UE_LOG(LogProgressComponent, Display, TEXT("LevelInfo: %s"), *PlayerState->GetLevelData(Attribute).ToString());
	}
}


void UProgressComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
