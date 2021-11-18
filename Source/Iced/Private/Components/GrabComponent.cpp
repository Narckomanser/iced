// Narckomanser's game


#include "Components/GrabComponent.h"

#include "BasePlayer.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogGrabComponent, All, All);

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

	//TODO rework it, added for test. Replace ByChannel to ByObject
	const auto Owner = GetOwner();
	FHitResult HitResult;

	FVector CameraLocation;
	FRotator CameraRotation;
	Cast<ABasePlayer>(Owner)->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	DrawDebugLine(GetWorld(),
	              Owner->GetActorLocation(),
	              CameraLocation + (CameraRotation.Vector() * 800),
	              FColor::Red,
	              false,
	              -1,
	              5,
	              2);

	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     Owner->GetActorLocation(),
	                                     CameraLocation + (CameraRotation.Vector() * 800),
	                                     ECC_Visibility);
	if (HitResult.Actor.Get())
	{
		UE_LOG(LogGrabComponent, Display, TEXT("%s detected"), *HitResult.GetActor()->GetName())
	}
}
