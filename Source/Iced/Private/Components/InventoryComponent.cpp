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

DEFINE_LOG_CATEGORY_STATIC(LogInventoryComponent, All, All);


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

	//TODO remove on release
	DevSpawnItems();
}

void UInventoryComponent::GrabSubscriber()
{
	const auto GrabComponent = Cast<ABasePlayer>(GetOwner())->GetGrabComponent();
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

//TODO refactor this method
void UInventoryComponent::Eqiup(ABaseItem* NewItem)
{
	// drop previous item
	// setup new item for use:
	//		1. set owner
	//		2. subscribe on events
	//	ignore collision with owner
	//	ignore collision with owner's items
	// attach to socket
	// add to equip map
	
	if (!NewItem) { return; }

	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto CombatComponent = Owner->GetCombatComponent();
	if (!CombatComponent) { return; }

	if (CombatComponent->DoesInBattle()) { return; }

	const auto ChangingItemType = NewItem->GetItemType();

	DropItem(ChangingItemType, Owner);
	// TODO this method->
	SetupItem(NewItem, Owner);

	
	SetupItemCollision(NewItem, Owner, true);

	const auto ParentMesh = Owner->GetMesh();
	if (!ParentMesh) { return; }

	AttachItemToSocket(NewItem, NewItem->GetItemSocket(), ParentMesh);

	const auto HitCapsule = NewItem->GetHitComponent();
	if (!HitCapsule) { return; }

	HitCapsule->OnComponentBeginOverlap.AddDynamic(NewItem,
	                                               &ABaseItem::OnComponentBeginOverlapHandle);

	NewItem->OnTakePointDamage.AddDynamic(NewItem, &ABaseItem::OnTakePointDamageHandle);

	Equipment[ChangingItemType] = NewItem;
}


void UInventoryComponent::DropItem(EItemTypes ItemType, ABasePlayer* Owner)
{
	// remove owner 
	// stop ignore owner
	// stop ignore owner's items
	// unsubscribe from event's
	// detach from owner's socket
	// in map set nullptr to item

	
	auto Item = Equipment[ItemType];
	if (!Item) { return; }


	//TODO move this method to other place
	SetupItemCollision(Equipment[ItemType], Owner, false);

	Item->OnTakePointDamage.RemoveAll(Item);
	Item->GetHitComponent()->OnComponentBeginOverlap.RemoveAll(Item);
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

void UInventoryComponent::SetupItem(ABaseItem* Item, AActor* NewOwner)
{
	Item->SetOwner(NewOwner);
	
}

// TODO FULLY REWORK THIS SHIT!!!! also need ignore owner's shield
void UInventoryComponent::SetupItemCollision(ABaseItem* Item, ABasePlayer* ItemOwner, bool ShouldIgnore)
{
	// const auto OwnerCollisionComponent = ItemOwner->GetCapsuleComponent();
	// if (!OwnerCollisionComponent) { return; }
	//
	// OwnerCollisionComponent->IgnoreActorWhenMoving(Item, ShouldIgnore);
	//
	// const auto OwnerMeshComponent = ItemOwner->GetMesh();
	// if (!OwnerMeshComponent) { return; }
	//
	// OwnerMeshComponent->IgnoreActorWhenMoving(Item, ShouldIgnore);


	

	const auto HitCapsule = Item->GetHitComponent();
	if (!HitCapsule) { return; }
	
	HitCapsule->IgnoreActorWhenMoving(ItemOwner, ShouldIgnore);

	for (const auto Equip : Equipment)
	{
		if (const auto IgnoredItem = Equip.Value)
		{
			IgnoredItem->GetHitComponent()->IgnoreActorWhenMoving(Cast<AActor>(Item), ShouldIgnore);
		}
	}
}

void UInventoryComponent::InitEquipmentList()
{
	for (EItemTypes ItemType : TEnumRange<EItemTypes>())
	{
		Equipment.Add(ItemType, nullptr);
	}
}

void UInventoryComponent::DevSpawnItems()
{
	for (const auto Item : DevBaseInventory)
	{
		const auto SpawnedItem = GetWorld()->SpawnActor<ABaseItem>(Item);
		Eqiup(SpawnedItem);
	}

	GetOwner<ABasePlayer>()->GetCombatComponent()->DevChangeStance();
}
