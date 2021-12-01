// Narckomanser's game


#include "Components/WeaponComponent.h"

#include "BasePlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "EquipFinishedAnimNotify.h"
#include "AttachItemAnimNotify.h"
#include "BaseItem.h"
#include "Components/CapsuleComponent.h"
#include "Notifies/NotifyUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

//TODO if add new items need to create one more component to hold them
//TODO add logic to equip and pick up other items(now only shield) ENUM???

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UseBattleMode(BattleMode);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimNotifies();

	Cast<ABasePlayer>(GetOwner())->OnGrabItem.AddUObject(this, &UWeaponComponent::Eqiup);
}

bool UWeaponComponent::CanEquip() const
{
	const auto Owner = GetOwner<ACharacter>();
	if (!Owner) return false;

	return !EquipInProgress && !Owner->GetMovementComponent()->IsFalling();
}

void UWeaponComponent::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	const auto Owner = GetOwner<ABasePlayer>();

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh || CharacterMesh != MeshComp) { return; }

	EquipInProgress = false;
	Owner->AllowMove(EMovementMode::MOVE_Walking);
}

void UWeaponComponent::OnAttachItem(USkeletalMeshComponent* MeshComp)
{
	const auto Owner = GetOwner<ACharacter>();

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh || CharacterMesh != MeshComp) { return; }

	AttachItemToSocket();
}

bool UWeaponComponent::ChangeBattleMode()
{
	BattleMode = !BattleMode;
	return BattleMode;
}

void UWeaponComponent::UseBattleMode(const bool Mode) const
{
	const auto Owner = GetOwner<ACharacter>();
	if (!Owner) return;

	Owner->bUseControllerRotationYaw = Mode;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = !Mode;
}

void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UWeaponComponent::Attack()
{
	//TODO forbid if not weapon, in air, in run(make separate method to check it)
	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!EquippedWeapon || !Owner) return;

	//TODO may be this method will be the wrapper, create other method which will play different AMs dependent on combo
	Owner->PlayAnimMontage(AnimList.AttackAnim);
}

void UWeaponComponent::ChangeStance()
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!CanEquip() || !Owner) return;

	EquipInProgress = true;
	Owner->AllowMove(EMovementMode::MOVE_None);

	UseBattleMode(ChangeBattleMode());

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh) { return; }

	CurrentEquipState = (++CurrentEquipState) % EquippedStateAnimInstances.Num();
	if (EquippedStateAnimInstances[CurrentEquipState])
	{
		CharacterMesh->SetAnimInstanceClass(EquippedStateAnimInstances[CurrentEquipState]);
	}

	if (EquipData[CurrentEquipAnimation].TransitionAnimation)
	{
		Owner->PlayAnimMontage(EquipData[CurrentEquipAnimation].TransitionAnimation);
		CurrentEquipAnimation = (++CurrentEquipAnimation) % EquipData.Num();
	}
}

void UWeaponComponent::InitAnimNotifies()
{
	for (const auto OneEquipData : EquipData)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(
			OneEquipData.TransitionAnimation);
		if (!EquipFinishedNotify) continue;

		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);

		const auto AttachWeaponNotify = FNotifyUtils::FindNotifyByClass<UAttachItemAnimNotify>(
			OneEquipData.TransitionAnimation);
		if (!AttachWeaponNotify) continue;

		AttachWeaponNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnAttachItem);
	}
}

void UWeaponComponent::Eqiup(AActor* NewWeapon)
{
	//TODO add conditions to add new weapon
	const auto ProbableWeapon = Cast<ABaseItem>(NewWeapon);
	if (!ProbableWeapon) { return; }

	DropEqippedWeapon();

	EquippedWeapon = ProbableWeapon;

	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) { return; }

	const auto OwnerCollisionComponent = Owner->FindComponentByClass<UCapsuleComponent>();
	const auto OwnerMeshComponent = Owner->FindComponentByClass<UMeshComponent>();
	if (!OwnerCollisionComponent || !OwnerMeshComponent) { return; }
	
	EquippedWeapon->FindComponentByClass<UMeshComponent>()->OnComponentBeginOverlap.AddDynamic(EquippedWeapon, &ABaseItem::OnComponentBeginOverlapHandle);

	EquippedWeapon->GetCollisionComponent()->IgnoreActorWhenMoving(GetOwner(), true);
	OwnerCollisionComponent->IgnoreActorWhenMoving(EquippedWeapon, true);
	OwnerMeshComponent->IgnoreActorWhenMoving(EquippedWeapon, true);

	AttachItemToSocket();
}

void UWeaponComponent::DropEqippedWeapon()
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!Owner || !EquippedWeapon) return;

	EquippedWeapon->OnActorHit.RemoveAll(EquippedWeapon);
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon = nullptr;
}

void UWeaponComponent::AttachItemToSocket()
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!Owner || !EquippedWeapon) return;

	//TODO if learn how to set collision with SM without offset and enable physics on SM - uncomment it
	//EquippedWeapon->DisableComponentsSimulatePhysics();

	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::SnapToTarget, false};
	EquippedWeapon->AttachToComponent(Owner->GetMesh(), AttachmentTransformRules,
	                                  EquipData[CurrentEquipAnimation].EquipSocketName);
}
