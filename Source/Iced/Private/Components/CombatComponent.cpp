// Narckomanser's game


#include "CombatComponent.h"
#include "BasePlayer.h"
#include "BaseItem.h"
#include "InventoryComponent.h"

#include "EquipFinishedAnimNotify.h"
#include "AttachItemAnimNotify.h"
#include "NotifyUtils.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogCombatComponent, All, All);

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	UseBattleMode(bBattleMode);
}

void UCombatComponent::DevChangeStance()
{
	ChangeStance();
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerInputComponent();
	InitAnimNotifies();
}

void UCombatComponent::SetupPlayerInputComponent()
{
	const auto InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) { return; }

	InputComponent->BindAction("Attack", IE_Pressed, this, &UCombatComponent::Attack);
	InputComponent->BindAction("ChangeStance", IE_Pressed, this, &UCombatComponent::ChangeStance);
}

bool UCombatComponent::CheckCalmState(const ABasePlayer* Owner, const ABaseItem* EquippedWeapon) const
{
	//TODO check IsRunning too???
	return !bDoesEquipInProgress && !Owner->GetMovementComponent()->IsFalling() && !EquippedWeapon->IsInCombatState();
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

//TODO create wrapper above the attack | attack will be the inner method | call wrappers in input events and set needed info to attack
void UCombatComponent::Attack()
{
	//TODO forbid jump when attack in progress
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto EquippedWeapon = Owner->GetInventoryComponent()->GetEquippedItem(EItemTypes::Weapon);
	if (!EquippedWeapon) { return; }
	
	if (!CheckCalmState(Owner, EquippedWeapon) || !bBattleMode) { return; }
	
	CombatStateSwitcher(EquippedWeapon);

	FTimerDelegate CombatEnablerDelegate;
	CombatEnablerDelegate.BindUFunction(this, FName("CombatStateSwitcher"), EquippedWeapon);
	
	const float AnimDuration = Owner->PlayAnimMontage(CombatAnimList[EAttackTypes::DefaultAttack]);
	GetWorld()->GetTimerManager().SetTimer(OverlapEnableTimer, CombatEnablerDelegate, AnimDuration, false);

	//don't forget change this place with anim
	OnAttack.Broadcast(EAttackTypes::DefaultAttack);
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
