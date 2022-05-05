// Narckomanser's game


#include "ProgressComponent.h"


UProgressComponent::UProgressComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UProgressComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UProgressComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
