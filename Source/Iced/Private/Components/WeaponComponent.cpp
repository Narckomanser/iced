// Narckomanser's game


#include "WeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "BasePlayer.h"
#include "BaseItem.h"
#include "GrabComponent.h"
#include "Notifies/NotifyUtils.h"
#include "EquipFinishedAnimNotify.h"
#include "AttachItemAnimNotify.h"
#include "AttackEndAnimNotify.h"

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

	SetupPlayerInputComponent();
	InitAnimNotifies();
	GrabSubscriber();
}

void UWeaponComponent::SetupPlayerInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (!InputComponent) { return; }

	InputComponent->BindAction("Attack", IE_Pressed, this, &UWeaponComponent::Attack);
	InputComponent->BindAction("ChangeStance", IE_Pressed, this, &UWeaponComponent::ChangeStance);
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
	//TODO forbid if not weapon, in attack, in air, in run(make separate method to check it)
	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!EquippedWeapon || !Owner) return;

	//TODO may be this method will be the wrapper, create other method which will play different AMs dependent on combo

	//TODO REWORK!!!
	EquippedWeapon->ChangeAttackState(Owner->GetMesh());
	
	WeaponOverlapEventEnabler();
	const float AnimDuration = Owner->PlayAnimMontage(CombatAnimList.AttackAnim);
	GetWorld()->GetTimerManager().SetTimer(OverlapEnableTimer, this, &UWeaponComponent::WeaponOverlapEventEnabler, AnimDuration);
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

void UWeaponComponent::GrabSubscriber()
{
	const auto GrabComponent = GetOwner()->FindComponentByClass<UGrabComponent>();
	if (!GrabComponent) { return; }

	GrabComponent->OnGrabItem.AddUObject(this, &UWeaponComponent::Eqiup);
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


//TODO fully rework this method
void UWeaponComponent::Eqiup(ABaseItem* NewWeapon)
{
	//TODO add conditions to add new weapon
	if (!NewWeapon) { return; }

	DropEqippedWeapon();

	EquippedWeapon = NewWeapon;
	
	const auto Owner = Cast<ACharacter>(GetOwner());
	if (!Owner) { return; }
	
	EquippedWeapon->SetOwner(Owner);

	const auto OwnerCollisionComponent = Owner->GetCapsuleComponent();
	const auto OwnerMeshComponent = Owner->GetMesh();
	if (!OwnerCollisionComponent || !OwnerMeshComponent) { return; }

	EquippedWeapon->GetHitCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(EquippedWeapon, &ABaseItem::OnComponentBeginOverlapHandle);
	
	OwnerCollisionComponent->IgnoreActorWhenMoving(EquippedWeapon, true);
	OwnerMeshComponent->IgnoreActorWhenMoving(EquippedWeapon, true);

	const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>(CombatAnimList.AttackAnim);
	AttackEndNotify->OnNotified.AddUObject(EquippedWeapon, &ABaseItem::ChangeAttackState);

	AttachItemToSocket();
}

//TODO rework it. may be need to move to weapon
void UWeaponComponent::DropEqippedWeapon()
{
	const auto Owner = Cast<ACharacter>(GetOwner());
	const auto AttackEndNotify = FNotifyUtils::FindNotifyByClass<UAttackEndAnimNotify>(CombatAnimList.AttackAnim);
	if (!Owner || !EquippedWeapon || !AttackEndNotify) return;

	EquippedWeapon->GetHitCapsuleComponent()->OnComponentBeginOverlap.RemoveAll(EquippedWeapon);
	AttackEndNotify->OnNotified.RemoveAll(EquippedWeapon);
	EquippedWeapon->SetOwner(nullptr);
	EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	EquippedWeapon = nullptr;
}

//TODO add item to arg
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

void UWeaponComponent::WeaponOverlapEventEnabler()
{
	const auto WeaponMeshComponent = EquippedWeapon->GetMesh();
	if (!WeaponMeshComponent) { return; }
	
	WeaponMeshComponent->SetGenerateOverlapEvents(!WeaponMeshComponent->GetGenerateOverlapEvents());
}
