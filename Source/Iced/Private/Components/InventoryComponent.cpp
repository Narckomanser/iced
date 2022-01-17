// Narckomanser's game


#include "InventoryComponent.h"
#include "GrabComponent.h"
#include "BaseItem.h"

#include "NotifyUtils.h"
#include "AttackEndAnimNotify.h"
#include "BasePlayer.h"
#include "CombatComponent.h"

#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InitEquipmentList();
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

	const auto CombatComponent = Owner->GetCombatComponent();
	if (!CombatComponent) { return; }

	if (CharacterMesh != MeshComp) { return; }

	AttachItemToSocket(Equipment[EItemTypes::Weapon], CombatComponent->GetStanceSocketName(), MeshComp);
}

void UInventoryComponent::Eqiup(ABaseItem* NewItem)
{
	if (!NewItem) { return; }
	
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto CombatComponent = Owner->GetCombatComponent();
	if (!CombatComponent) { return; }

	if (CombatComponent->DoesInBattle()) { return; }

	const auto ChangingItemType = NewItem->GetItemType();

	DropItem(ChangingItemType, Owner, CombatComponent->GetAnimList());

	Equipment[ChangingItemType] = NewItem;
	Equipment[ChangingItemType]->SetOwner(Owner);
	SetupEquippedItem(Equipment[ChangingItemType], Owner, true);
	InitNotifies(CombatComponent->GetAnimList());

	const auto ParentMesh = Owner->GetMesh();
	if (!ParentMesh) { return; }

	AttachItemToSocket(Equipment[ChangingItemType], NewItem->GetItemSocket(), ParentMesh);

	const auto HitCapsule = Equipment[NewItem->GetItemType()]->GetHitCapsuleComponent();
	if (!HitCapsule) { return; }

	HitCapsule->OnComponentBeginOverlap.AddDynamic(Equipment[ChangingItemType],
	                                               &ABaseItem::OnComponentBeginOverlapHandle);
}


void UInventoryComponent::DropItem(EItemTypes ItemType, ABasePlayer* Owner, const TArray<UAnimMontage*>& AnimList)
{
	ABaseItem* Item = Equipment[ItemType];
	if (!Item) { return; }


	//TODO move this method to other place
	RemoveNotifies(AnimList);

	SetupEquippedItem(Equipment[ItemType], Owner, false);

	Item->GetHitCapsuleComponent()->OnComponentBeginOverlap.RemoveAll(Item);
	Item->SetOwner(nullptr);
	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	Item = nullptr;
}

void UInventoryComponent::AttachItemToSocket(ABaseItem* Item, const FName SocketName,
                                             USkeletalMeshComponent* CombatComponent) const
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Item || !Owner) return;

	//TODO setup collision and uncomment it
	//Used to ignore actors after death
	//Item->DisableComponentsSimulatePhysics();

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::SnapToTarget, false};
	Item->AttachToComponent(CombatComponent, AttachmentTransformRules, SocketName);
}

void UInventoryComponent::InitNotifies(const TArray<UAnimMontage*>& AnimList)
{
	for (const auto Anim : AnimList)
	{
		if (!Anim) continue;

		const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>(Anim);
		//TODO remove hardcoded item ptr
		AttackEndNotify->OnNotified.AddUObject(Equipment[EItemTypes::Weapon], &ABaseItem::ChangeCombatState);
	}
}

void UInventoryComponent::RemoveNotifies(const TArray<UAnimMontage*>& AnimList) const
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto CombatComponent = Owner->GetCombatComponent();
	if (!CombatComponent) { return; }

	for (const auto Anim : AnimList)
	{
		if (!Anim) continue;

		const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>(Anim);
		//TODO remove hardcoded item ptr
		AttackEndNotify->OnNotified.RemoveAll(Equipment[EItemTypes::Weapon]);
	}
}

void UInventoryComponent::SetupEquippedItem(ABaseItem* Item, ABasePlayer* ItemOwner, bool ShouldIgnore)
{
	const auto OwnerCollisionComponent = ItemOwner->GetCapsuleComponent();
	if (!OwnerCollisionComponent) { return; }

	OwnerCollisionComponent->IgnoreActorWhenMoving(Item, ShouldIgnore);

	const auto OwnerMeshComponent = ItemOwner->GetMesh();
	if (!OwnerMeshComponent) { return; }

	OwnerMeshComponent->IgnoreActorWhenMoving(Item, ShouldIgnore);

	const auto HitCapsule = Item->GetHitCapsuleComponent();
	if (!HitCapsule) { return; }

	HitCapsule->IgnoreActorWhenMoving(ItemOwner, ShouldIgnore);
}

void UInventoryComponent::InitEquipmentList()
{
	for (EItemTypes ItemType : TEnumRange<EItemTypes>())
	{
		Equipment.Add(ItemType, nullptr);
	}
}
