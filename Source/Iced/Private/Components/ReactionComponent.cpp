// Narckomanser's game


#include "Components/ReactionComponent.h"

#include "GameFramework/Character.h"

UReactionComponent::UReactionComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UReactionComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UReactionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UReactionComponent::GetResponseForDamage(const UDamageType* DamageType)
{
	if (const auto Owner = GetOwner<ACharacter>(); DamageType && Owner)
	{
		Owner->PlayAnimMontage(ResponseMap[DamageType->GetClass()]);
	}
}
