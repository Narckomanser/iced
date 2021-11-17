// Narckomanser's game


#include "Components/WeaponComponent.h"

#include "BasePlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Notifies/EquipFinishedAnimNotify.h"
#include "Notifies/NotifyUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	UseBattleMode(BattleMode);
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	InitAnimNotifies();
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
	UE_LOG(LogWeaponComponent, Display, TEXT("Attack Pressed"));
}

void UWeaponComponent::Equip()
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

	if (EquipAnimations[CurrentEquipAnimation])
	{
		Owner->PlayAnimMontage(EquipAnimations[CurrentEquipAnimation]);
		CurrentEquipAnimation = (++CurrentEquipAnimation) % EquipAnimations.Num();
	}
}

void UWeaponComponent::InitAnimNotifies()
{
	for (const auto EquipAnimation : EquipAnimations)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimation);
		if (!EquipFinishedNotify) continue;

		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
	}
}
