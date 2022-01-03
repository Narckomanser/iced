// Narckomanser's game


#include "WeaponComponent.h"
#include "BasePlayer.h"
#include "BaseItem.h"
#include "GrabComponent.h"
#include "InventoryComponent.h"

#include "EquipFinishedAnimNotify.h"
#include "AttachItemAnimNotify.h"
#include "NotifyUtils.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"


DEFINE_LOG_CATEGORY_STATIC(LogWeaponComponent, All, All);

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	UseBattleMode(BattleMode);
	InitCombatAnimList();
}

void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();

	SetupPlayerInputComponent();
	InitAnimNotifies();
}

void UWeaponComponent::SetupPlayerInputComponent()
{
	const auto InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) { return; }

	InputComponent->BindAction("Attack", IE_Pressed, this, &UWeaponComponent::Attack);
	InputComponent->BindAction("ChangeStance", IE_Pressed, this, &UWeaponComponent::ChangeStance);
}

bool UWeaponComponent::CanChangeStance() const
{
	const auto Owner = GetOwner<ACharacter>();
	if (!Owner) { return false; }

	return !EquipInProgress && !Owner->GetMovementComponent()->IsFalling();
}

void UWeaponComponent::OnStanceChanged(USkeletalMeshComponent* MeshComp)
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

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

TArray<UAnimMontage*> UWeaponComponent::GetAnimList() const
{
	TArray<UAnimMontage*> OutArray;
	CombatAnimList.GenerateValueArray(OutArray);

	return OutArray;
}

void UWeaponComponent::Attack()
{
	//TODO forbid if not weapon, in attack, in air, in run(make separate method to check it)
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto EquippedWeapon = Owner->GetInventoryComponent()->GetEquippedWeapon();
	if (!EquippedWeapon) { return; }

	EquippedWeapon->ChangeAttackState(Owner->GetMesh());

	WeaponOverlapEventEnabler();

	//TODO define needed anim and send it to PlayAnimMontage
	const float AnimDuration = Owner->PlayAnimMontage(CombatAnimList[EAttackTypes::DefaultAttack]);
	GetWorld()->GetTimerManager().SetTimer(OverlapEnableTimer, this, &UWeaponComponent::WeaponOverlapEventEnabler,
	                                       AnimDuration);
}

void UWeaponComponent::InitAnimNotifies()
{
	for (const auto OneEquipData : EquipData)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(
			OneEquipData.TransitionAnimation);
		if (!EquipFinishedNotify) continue;

		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnStanceChanged);

		const auto Owner = GetOwner<ABasePlayer>();
		if (!Owner) { return; }

		const auto InventoryComponent = Owner->GetInventoryComponent();

		const auto AttachWeaponNotify = FNotifyUtils::FindNotifyByClass<UAttachItemAnimNotify>(
			OneEquipData.TransitionAnimation);
		if (!AttachWeaponNotify || !InventoryComponent) continue;

		AttachWeaponNotify->OnNotified.AddUObject(InventoryComponent, &UInventoryComponent::OnAttachItem);
	}
}

void UWeaponComponent::ChangeStance()
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!CanChangeStance() || !Owner) return;

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

	if (EquipData[CurrentStanceState].TransitionAnimation)
	{
		Owner->PlayAnimMontage(EquipData[CurrentStanceState].TransitionAnimation);
		CurrentStanceState = (++CurrentStanceState) % EquipData.Num();
	}
}

void UWeaponComponent::WeaponOverlapEventEnabler() const
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto EquippedWeapon = Owner->GetInventoryComponent()->GetEquippedWeapon();
	if (!EquippedWeapon) { return; }

	const auto WeaponMeshComponent = EquippedWeapon->GetMesh();
	if (!WeaponMeshComponent) { return; }

	WeaponMeshComponent->SetGenerateOverlapEvents(!WeaponMeshComponent->GetGenerateOverlapEvents());
}

void UWeaponComponent::InitCombatAnimList()
{
	for (EAttackTypes AttackType : TEnumRange<EAttackTypes>())
	{
		CombatAnimList.Add(AttackType, nullptr);
	}
}
