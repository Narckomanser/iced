// Narckomanser's game


#include "Components/GrabComponent.h"

#include "DrawDebugHelpers.h"

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const auto Owner = GetOwner();

	DrawDebugLine(GetWorld(), Owner->GetActorLocation(), Owner->GetActorLocation() + (Owner->GetActorForwardVector() * 100), FColor::Red, false,
	              -1, 5, 10);
}
