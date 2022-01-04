// Narckomanser's game


#include "InventoryComponent.h"
#include "GrabComponent.h"
#include "BaseItem.h"

#include "NotifyUtils.h"
#include "AttackEndAnimNotify.h"
#include "BasePlayer.h"
#include "WeaponComponent.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"

UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                        FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	GrabSubscriber();
}

void UInventoryComponent::GrabSubscriber()
{
	const auto GrabComponent = GetOwner()->FindComponentByClass<UGrabComponent>();
	if (!GrabComponent) { return; }

	GrabComponent->OnGrabItem.AddUObject(this, &UInventoryComponent::Eqiup);
}

void UInventoryComponent::OnAttachItem(USkeletalMeshComponent* MeshComp)
{
	const auto Owner = GetOwner<ABasePlayer>();

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh) { return; }

	const auto WeaponComponent = Owner->GetWeaponComponent();
	if (!WeaponComponent) { return; }

	if (CharacterMesh != MeshComp) { return; }

	AttachItemToSocket(MeshComp, WeaponComponent->GetStanceSocketName());
}

void UInventoryComponent::Eqiup(ABaseItem* NewWeapon)
{
	//TODO add conditions to add new weapon
	if (!NewWeapon) { return; }

	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto WeaponComponent = Owner->GetWeaponComponent();
	if (!WeaponComponent) { return; }

	DropEqippedWeapon(Owner, WeaponComponent);

	EquippedWeapon = NewWeapon;
	EquippedWeapon->SetOwner(Owner);
	SetupEquippedItem(Owner, true);
	InitNotifies(WeaponComponent->GetAnimList());

	const auto ParentMesh = Owner->GetMesh();
	if (!ParentMesh) { return; }

	AttachItemToSocket(ParentMesh, WeaponComponent->GetStanceSocketName());

	const auto WeaponHitCapsule = EquippedWeapon->GetHitCapsuleComponent();
	if (!WeaponHitCapsule) { return; }

	WeaponHitCapsule->OnComponentBeginOverlap.AddDynamic(EquippedWeapon, &ABaseItem::OnComponentBeginOverlapHandle);
}


void UInventoryComponent::DropEqippedWeapon(ABasePlayer* Owner, const UWeaponComponent* WeaponComponent)
{
	if (!EquippedWeapon) { return; }

	RemoveNotifies(WeaponComponent->GetAnimList());
	
	SetupEquippedItem(Owner, false);
	
	EquippedWeapon->GetHitCapsuleComponent()->OnComponentBeginOverlap.RemoveAll(EquippedWeapon);
	EquippedWeapon->SetOwner(nullptr);
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon = nullptr;
}

//TODO add item to arg
void UInventoryComponent::AttachItemToSocket(USkeletalMeshComponent* WeaponComponent, const FName SocketName) const
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!EquippedWeapon || !Owner) return;

	//TODO setup collision and uncomment it
	//Used to ignore actors after death
	//EquippedWeapon->DisableComponentsSimulatePhysics();

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::SnapToTarget, false};
	EquippedWeapon->AttachToComponent(WeaponComponent, AttachmentTransformRules, SocketName);
}

void UInventoryComponent::InitNotifies(const TArray<UAnimMontage*>& AnimList)
{
	for (const auto Anim : AnimList)
	{
		if (!Anim) continue;

		const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>(Anim);
		AttackEndNotify->OnNotified.AddUObject(EquippedWeapon, &ABaseItem::ChangeAttackState);
	}
}

void UInventoryComponent::RemoveNotifies(const TArray<UAnimMontage*>& AnimList) const
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto WeaponComponent = Owner->GetWeaponComponent();
	if (!WeaponComponent) { return; }

	for (const auto Anim : AnimList)
	{
		if (!Anim) continue;

		const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>(Anim);
		AttackEndNotify->OnNotified.RemoveAll(EquippedWeapon);
	}
}

void UInventoryComponent::SetupEquippedItem(ABasePlayer* ItemOwner, bool ShouldIgnore)
{
	const auto OwnerCollisionComponent = ItemOwner->GetCapsuleComponent();
	if (!OwnerCollisionComponent) { return; }

	OwnerCollisionComponent->IgnoreActorWhenMoving(EquippedWeapon, ShouldIgnore);

	const auto OwnerMeshComponent = ItemOwner->GetMesh();
	if (!OwnerMeshComponent) { return; }

	OwnerMeshComponent->IgnoreActorWhenMoving(EquippedWeapon, ShouldIgnore);

	const auto WeaponHitCapsule = EquippedWeapon->GetHitCapsuleComponent();
	if (!WeaponHitCapsule) { return; }

	WeaponHitCapsule->IgnoreActorWhenMoving(ItemOwner, ShouldIgnore);
}
