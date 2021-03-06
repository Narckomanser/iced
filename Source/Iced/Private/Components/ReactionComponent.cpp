// Narckomanser's game


#include "Components/ReactionComponent.h"

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

UAnimMontage* UReactionComponent::GetResponseAnimation(const UDamageType* DamageType)
{
	//TODO different reaction by hitted actor(shield and body)
	return ResponseList[DamageType->GetClass()];
}
