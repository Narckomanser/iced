// Narckomanser's game


#include "Components/GrabComponent.h"
#include "BasePlayer.h"
#include "BaseItem.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/SpringArmComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogGrabComponent, All, All);

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();
}

AActor* UGrabComponent::DetectItem() const
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     GetStartPoint(),
	                                     GetEndPoint(),
	                                     ECC_Visibility);

	const auto Founded = HitResult.GetActor();

	return Founded->IsA(ABaseItem::StaticClass()) ? Founded : nullptr;
}

FVector UGrabComponent::GetStartPoint() const
{
	const auto Owner = Cast<ABasePlayer>(GetOwner());

	return Owner ? Owner->GetMesh()->GetBoneLocation(BoneName) : FVector{};
}

FVector UGrabComponent::GetEndPoint() const
{
	const auto Owner = Cast<ABasePlayer>(GetOwner());
	if (!Owner) return FVector{};

	const auto SpringArmComponent = Owner->FindComponentByClass<USpringArmComponent>();
	if (!SpringArmComponent) return FVector{};

	FVector CameraLocation;
	FRotator CameraRotation;
	Owner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	return CameraLocation + (CameraRotation.Vector() * (SpringArmComponent->
		TargetArmLength + GrabDistance));
}

void UGrabComponent::GrabItem() const
{
	//TODO do something with founded item
	if (const auto Found = Cast<ABaseItem>(DetectItem()))
	{
		const auto Owner = Cast<ABasePlayer>(GetOwner());
		if (!Owner) return;

		Owner->TakeItem(Found);
	}
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	GrabItem();
}
