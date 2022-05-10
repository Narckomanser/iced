// Narckomanser's game


#include "Components/GrabComponent.h"

#include "BaseItem.h"
#include "BasePlayer.h"
#include "GameFramework/SpringArmComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogGrabComponent, All, All);

UGrabComponent::UGrabComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerInputComponent();
}

void UGrabComponent::SetupPlayerInputComponent()
{
	const auto InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) { return; }

	InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabComponent::Grab);
}

ABaseItem* UGrabComponent::DetectItem() const
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult,
	                                     GetStartPoint(),
	                                     GetEndPoint(),
	                                     ECC_Visibility);
	if (const auto FoundedActor = HitResult.GetActor())
	{
		UE_LOG(LogGrabComponent, Display, TEXT("Founded object: %s"), *FoundedActor->GetName());
	}

	return Cast<ABaseItem>(HitResult.GetActor());
}

FVector UGrabComponent::GetStartPoint() const
{
	const auto Owner = GetOwner<ABasePlayer>();

	return Owner ? Owner->GetMesh()->GetBoneLocation(BoneName) : FVector{};
}

FVector UGrabComponent::GetEndPoint() const
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) return FVector{};

	const auto SpringArmComponent = Owner->FindComponentByClass<USpringArmComponent>();
	if (!SpringArmComponent) return FVector{};

	FVector CameraLocation;
	FRotator CameraRotation;
	Owner->GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);

	return CameraLocation + (CameraRotation.Vector() * (SpringArmComponent->
		TargetArmLength + GrabDistance));
}

void UGrabComponent::Grab()
{
	if (const auto Owner = GetOwner<ABasePlayer>(); Owner)
	{
		Owner->OnGrabItem.Broadcast(DetectItem());
	}
}

void UGrabComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//TODO uncomment after ui implemented
	//DetectItem();
}
