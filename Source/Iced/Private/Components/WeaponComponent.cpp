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
	
	Cast<ABasePlayer>(GetOwner())->OnGrabItem.AddUObject(this, &UWeaponComponent::EqiupWeapon);
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

	if (EquipData[CurrentEquipAnimation].TransitionAnimation)
	{
		Owner->PlayAnimMontage(EquipData[CurrentEquipAnimation].TransitionAnimation);
		CurrentEquipAnimation = (++CurrentEquipAnimation) % EquipData.Num();
	}

	//TODO reattach weapon to current state socket
}

void UWeaponComponent::InitAnimNotifies()
{
	for (const auto OneEquipData : EquipData)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(OneEquipData.TransitionAnimation);
		if (!EquipFinishedNotify) continue;

		EquipFinishedNotify->OnNotified.AddUObject(this, &UWeaponComponent::OnEquipFinished);
	}
}

void UWeaponComponent::EqiupWeapon(AActor* NewWeapon)
{
	//TODO delete old weapon
	
	EquippedWeapon = NewWeapon;
	if (!EquippedWeapon) { return; }

	const auto Owner = Cast<ACharacter>(GetOwner());
	if(!Owner) return;

	UE_LOG(LogWeaponComponent, Display, TEXT("ItemToGrab: %s"), *EquippedWeapon->GetName());

	//TODO if learn how to set collision with SM without offset and enable physics on SM - uncomment it
	//EquippedWeapon->DisableComponentsSimulatePhysics();
	
	const FAttachmentTransformRules AttachmentTransformRules{EAttachmentRule::SnapToTarget, false};
	EquippedWeapon->AttachToComponent(Owner->GetMesh(), AttachmentTransformRules, EquipData[CurrentEquipAnimation].EquipSocketName);
}
