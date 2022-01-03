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
	const auto WeaponComponent = Owner->GetWeaponComponent();
	if (!CharacterMesh || CharacterMesh != MeshComp || !WeaponComponent) { return; }

	AttachItemToSocket(WeaponComponent->GetStanceSocketName());
}

//TODO fully rework this method
void UInventoryComponent::Eqiup(ABaseItem* NewWeapon)
{
	//TODO add conditions to add new weapon
	if (!NewWeapon) { return; }

	DropEqippedWeapon();

	EquippedWeapon = NewWeapon;

	const auto Owner = Cast<ABasePlayer>(GetOwner());
	if (!Owner) { return; }

	EquippedWeapon->SetOwner(Owner);

	const auto OwnerCollisionComponent = Owner->GetCapsuleComponent();
	const auto OwnerMeshComponent = Owner->GetMesh();
	const auto WeaponHitCapsule = EquippedWeapon->GetHitCapsuleComponent();
	if (!OwnerCollisionComponent || !OwnerMeshComponent || !WeaponHitCapsule) { return; }

	WeaponHitCapsule->OnComponentBeginOverlap.AddDynamic(EquippedWeapon, &ABaseItem::OnComponentBeginOverlapHandle);

	OwnerCollisionComponent->IgnoreActorWhenMoving(EquippedWeapon, true);
	OwnerMeshComponent->IgnoreActorWhenMoving(EquippedWeapon, true);
	WeaponHitCapsule->IgnoreActorWhenMoving(Owner, true);

	const auto WeaponComponent = Owner->GetWeaponComponent();
	if (!WeaponComponent) { return; }

	InitNotifiesAfterEquip(WeaponComponent->GetAnimList());

	AttachItemToSocket(WeaponComponent->GetStanceSocketName());
}


void UInventoryComponent::DropEqippedWeapon()
{
	const auto WeaponComponent = Cast<ABasePlayer>(GetOwner())->GetWeaponComponent();
	if (!WeaponComponent) { return; }

	const auto Owner = Cast<ACharacter>(GetOwner());
	//TODO create method to unitiate notifies
	const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>();
	if (!Owner || !EquippedWeapon || !AttackEndNotify) return;

	EquippedWeapon->GetHitCapsuleComponent()->OnComponentBeginOverlap.RemoveAll(EquippedWeapon);
	AttackEndNotify->OnNotified.RemoveAll(EquippedWeapon);
	EquippedWeapon->SetOwner(nullptr);
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon = nullptr;
}

//TODO add item to arg
void UInventoryComponent::AttachItemToSocket(const FName SocketName) const
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!Owner || !EquippedWeapon) return;

	//TODO if learn how to set collision with SM without offset and enable physics on SM - uncomment it
	//Used to ignore actors after death
	//EquippedWeapon->DisableComponentsSimulatePhysics();

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::SnapToTarget, false};
	EquippedWeapon->AttachToComponent(Owner->GetMesh(), AttachmentTransformRules, SocketName);
}

void UInventoryComponent::InitNotifiesAfterEquip(const TArray<UAnimMontage*>& AnimList)
{
	const auto WeaponComponent = Cast<ABasePlayer>(GetOwner())->GetWeaponComponent();
	if (!WeaponComponent) { return; }

	for (const auto Anim : AnimList)
	{
		const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>(Anim);
		AttackEndNotify->OnNotified.AddUObject(EquippedWeapon, &ABaseItem::ChangeAttackState);
	}
}
