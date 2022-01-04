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

	UseBattleMode(bBattleMode);
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

bool UWeaponComponent::CheckCalmState() const
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return false; }

	const auto EquippedWeapon = Owner->GetInventoryComponent()->GetEquippedWeapon();
	if (!EquippedWeapon) { return false; }

	//TODO check IsRunning too???
	return !bDoesEquipInProgress && !Owner->GetMovementComponent()->IsFalling() && !EquippedWeapon->IsInAttackState();
}

void UWeaponComponent::OnStanceChanged(USkeletalMeshComponent* MeshComp)
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh || CharacterMesh != MeshComp) { return; }

	bDoesEquipInProgress = false;
	Owner->AllowMove(EMovementMode::MOVE_Walking);
}

bool UWeaponComponent::ChangeBattleMode()
{
	bBattleMode = !bBattleMode;
	return bBattleMode;
}

void UWeaponComponent::UseBattleMode(const bool bMode) const
{
	const auto Owner = GetOwner<ACharacter>();
	if (!Owner) return;

	Owner->bUseControllerRotationYaw = bMode;
	Owner->GetCharacterMovement()->bOrientRotationToMovement = !bMode;
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
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }

	const auto EquippedWeapon = Owner->GetInventoryComponent()->GetEquippedWeapon();
	if (!EquippedWeapon) { return; }
	
	if (!CheckCalmState() || !bBattleMode) { return; }

	EquippedWeapon->ChangeAttackState(Owner->GetMesh());

	WeaponOverlapEventEnabler();

	//TODO define needed anim and send it to PlayAnimMontage
	const float AnimDuration = Owner->PlayAnimMontage(CombatAnimList[EAttackTypes::DefaultAttack]);
	GetWorld()->GetTimerManager().SetTimer(OverlapEnableTimer, this, &UWeaponComponent::WeaponOverlapEventEnabler,
	                                       AnimDuration);
}

void UWeaponComponent::InitAnimNotifies()
{
	for (const auto OneStanceData : StanceData)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(
			OneStanceData.TransitionAnimation);
		if (!EquipFinishedNotify) continue;

		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnStanceChanged);

		const auto Owner = GetOwner<ABasePlayer>();
		if (!Owner) { return; }

		const auto InventoryComponent = Owner->GetInventoryComponent();

		const auto AttachWeaponNotify = FNotifyUtils::FindNotifyByClass<UAttachItemAnimNotify>(
			OneStanceData.TransitionAnimation);
		if (!AttachWeaponNotify || !InventoryComponent) continue;

		AttachWeaponNotify->OnNotified.AddUObject(InventoryComponent, &UInventoryComponent::OnAttachItem);
	}
}

void UWeaponComponent::ChangeStance()
{
	const auto Owner = GetOwner<ABasePlayer>();
	if (!Owner) { return; }
	
	if (!CheckCalmState()) { return; }

	bDoesEquipInProgress = true;
	Owner->AllowMove(EMovementMode::MOVE_None);

	UseBattleMode(ChangeBattleMode());

	const auto CharacterMesh = Owner->GetMesh();
	if (!CharacterMesh) { return; }

	if (const auto AnimInstance = StanceData[CurrentStanceState].StanceAnimInstance)
	{
		CharacterMesh->SetAnimInstanceClass(AnimInstance);
	}

	if (const auto TransitionAnim = StanceData[CurrentStanceState].TransitionAnimation)
	{
		Owner->PlayAnimMontage(TransitionAnim);
	}

	CurrentStanceState = (++CurrentStanceState) % StanceData.Num();
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
