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

void UInventoryComponent::Eqiup(ABaseItem* NewItem)
{
	if (!NewItem) { return; }

	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto CombatComponent = Owner->GetCombatComponent();
	if (!CombatComponent) { return; }

	if (CombatComponent->DoesInBattle()) { return; }

	const auto ChangingItemType = NewItem->GetItemType();

	DropItem(ChangingItemType);

	SetupItem(NewItem, Owner);
	SetupItemCollision(NewItem, Owner, true);

	const auto ParentMesh = Owner->GetMesh();
	if (!ParentMesh) { return; }

	AttachItemToSocket(NewItem, NewItem->GetItemSocket(), ParentMesh);

	Equipment[ChangingItemType] = NewItem;
}


void UInventoryComponent::DropItem(EItemTypes ItemType)
{
	auto Item = Equipment[ItemType];
	if (!Item) { return; }

	const auto ItemOwner = Cast<ABasePlayer>(Item->GetOwner());
	if (!ItemOwner) { return; }

	SetupItemCollision(Equipment[ItemType], ItemOwner, false);

	Item->OnTakePointDamage.Remove(Item, "OnTakePointDamageHandle");
	Item->GetHitComponent()->OnComponentBeginOverlap.Remove(Item, "OnComponentBeginOverlapHandle");

	Item->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	Item->SetOwner(nullptr);
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

	const auto HitCapsule = Item->GetHitComponent();
	if (!HitCapsule) { return; }

	HitCapsule->OnComponentBeginOverlap.AddDynamic(Item,
	                                               &ABaseItem::OnComponentBeginOverlapHandle);

	Item->OnTakePointDamage.AddDynamic(Item, &ABaseItem::OnTakePointDamageHandle);
}

void UInventoryComponent::SetupItemCollision(ABaseItem* Item, ABasePlayer* ItemOwner, bool ShouldIgnore)
{
	const auto HitCapsule = Item->GetHitComponent();
	if (!HitCapsule) { return; }

	HitCapsule->IgnoreActorWhenMoving(ItemOwner, ShouldIgnore);

	for (const auto Equip : Equipment)
	{
		if (const auto IgnoredItem = Equip.Value)
		{
			Item->GetHitComponent()->IgnoreActorWhenMoving(Cast<AActor>(IgnoredItem), ShouldIgnore);
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
