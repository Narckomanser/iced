// Narckomanser's game


#include "CombatComponent.h"
#include "BasePlayer.h"
#include "BaseItem.h"
#include "InventoryComponent.h"

#include "EquipFinishedAnimNotify.h"
#include "AttachItemAnimNotify.h"
#include "NotifyUtils.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogCombatComponent, All, All);

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	InitCombatAnimMap();
}

void UCombatComponent::DevChangeStance()
{
	ChangeStance();
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerInputComponent();
	UseBattleMode(bBattleMode);
	InitAnimNotifies();
}

void UCombatComponent::SetupPlayerInputComponent()
{
	const auto InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) { return; }

	InputComponent->BindAction("DefaultAttack", IE_Pressed, this, &UCombatComponent::DefaultAttack);
	InputComponent->BindAction("MightAttack", IE_Pressed, this, &UCombatComponent::MightAttack);
	
	InputComponent->BindAction("ChangeStance", IE_Pressed, this, &UCombatComponent::ChangeStance);
}

bool UCombatComponent::CheckCalmState(const ABasePlayer* Owner, const ABaseItem* EquippedWeapon) const
{
	return !bDoesEquipInProgress && !Owner->GetMovementComponent()->IsFalling() && !Owner->IsRunning() && !EquippedWeapon->IsInCombatState();
}

void UCombatComponent::OnStanceChanged(USkeletalMeshComponent* MeshComp)
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh || CharacterMesh != MeshComp) { return; }

	bDoesEquipInProgress = false;
	Owner->AllowMove(EMovementMode::MOVE_Walking);
}

bool UCombatComponent::ChangeBattleMode()
{
	bBattleMode = !bBattleMode;
	return bBattleMode;
}

void UCombatComponent::UseBattleMode(const bool bMode) const
{
	const auto Owner = GetOwner<ACharacter>();
	if (!Owner) return;

	Owner->bUseControllerRotationYaw = bMode;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = !bMode;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UCombatComponent::DefaultAttack()
{
	MakeStrike(EAttackTypes::DefaultAttack);
}

void UCombatComponent::MightAttack()
{
	MakeStrike(EAttackTypes::MightAttack);
}

void UCombatComponent::MakeStrike(const EAttackTypes AttackType)
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto EquippedWeapon = Owner->GetInventoryComponent()->GetEquippedItem(EItemTypes::Weapon);
	if (!EquippedWeapon) { return; }
	
	if (!CheckCalmState(Owner, EquippedWeapon) || !bBattleMode) { return; }
	
	CombatStateSwitcher(EquippedWeapon);

	FTimerDelegate CombatEnablerDelegate;
	CombatEnablerDelegate.BindUFunction(this, FName("CombatStateSwitcher"), EquippedWeapon);
	
	const float AnimDuration = Owner->PlayAnimMontage(CombatAnimMap[AttackType]);
	GetWorld()->GetTimerManager().SetTimer(OverlapEnableTimer, CombatEnablerDelegate, AnimDuration, false);

	Owner->OnAttack.Broadcast(AttackType);
}

void UCombatComponent::InitAnimNotifies()
{
	for (const auto OneStanceData : StanceData)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(
			OneStanceData.TransitionAnimation);
		if (!EquipFinishedNotify) continue;

		EquipFinishedNotify->OnNotified.AddUObject(this, &UCombatComponent::OnStanceChanged);

		const auto Owner = GetOwner<ABasePlayer>();
		if (!Owner) { return; }

		const auto InventoryComponent = Owner->GetInventoryComponent();

		const auto AttachWeaponNotify = FNotifyUtils::FindNotifyByClass<UAttachItemAnimNotify>(
			OneStanceData.TransitionAnimation);
		if (!AttachWeaponNotify || !InventoryComponent) continue;

		AttachWeaponNotify->OnNotified.AddUObject(InventoryComponent, &UInventoryComponent::OnAttachItem);
	}
}

void UCombatComponent::ChangeStance()
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto EquippedWeapon = Owner->GetInventoryComponent()->GetEquippedItem(EItemTypes::Weapon);
	if (!EquippedWeapon) { return; }
	
	if (!CheckCalmState(Owner, EquippedWeapon)) { return; }

	bDoesEquipInProgress = true;
	Owner->AllowMove(EMovementMode::MOVE_None);

	UseBattleMode(ChangeBattleMode());

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh) { return; }

	if (const auto AnimInstance = StanceData[CurrentStanceState].StanceAnimInstance; AnimInstance)
	{
		CharacterMesh->SetAnimInstanceClass(AnimInstance);
	}

	if (const auto TransitionAnim = StanceData[CurrentStanceState].TransitionAnimation; TransitionAnim)
	{
		Owner->PlayAnimMontage(TransitionAnim);
	}

	CurrentStanceState = (++CurrentStanceState) % StanceData.Num();
}

void UCombatComponent::CombatStateSwitcher(ABaseItem* Item) const
{
	if (!Item) { return; }

	const auto WeaponMeshComponent = Item->GetMesh();
	if (!WeaponMeshComponent) { return; }

	WeaponMeshComponent->SetGenerateOverlapEvents(!WeaponMeshComponent->GetGenerateOverlapEvents());

	Item->ChangeCombatState();
}

void UCombatComponent::InitCombatAnimMap()
{
	for (EAttackTypes AttackType : TEnumRange<EAttackTypes>())
	{
		CombatAnimMap.Add(AttackType, nullptr);
	}
}
