#include "Player/BasePlayer.h"

#include "PlayerMovementComponent.h"
#include "HealthComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Notifies/EquipFinishedAnimNotify.h"
#include "Notifies/NotifyUtils.h"
#include "WeaponComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePlayer, All, All);

ABasePlayer::ABasePlayer(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	WeaponComponent->CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");

	UseBattleMode(BattleMode);

	// remove after UI implemented
	HealthText = CreateDefaultSubobject<UTextRenderComponent>("Health Text");
	HealthText->SetupAttachment(GetRootComponent());
}

float ABasePlayer::GetMovementDirection() const
{
	if (GetVelocity().IsZero()) return 0.f;

	const FVector VelocityNormal = GetVelocity().GetSafeNormal();
	const float AngleBetween = FMath::Acos(FVector::DotProduct(GetActorForwardVector(), VelocityNormal));
	const FVector CrossProduct = FVector::CrossProduct(GetActorForwardVector(), VelocityNormal);
	const float Degrees = FMath::RadiansToDegrees(AngleBetween);

	return CrossProduct.IsZero() ? Degrees : Degrees * FMath::Sign(CrossProduct.Z);
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();

	InitAnimNotifies();

	HealthComponent->OnDeath.AddUObject(this, &ABasePlayer::OnDeath);
}

void ABasePlayer::MoveForward(const float Amount)
{
	const FVector ForwardVector(UKismetMathLibrary::GetForwardVector(GetYawBasedRotator()));
	AddMovementInput(ForwardVector, Amount);
}

void ABasePlayer::MoveRight(const float Amount)
{
	const FVector RightVector(UKismetMathLibrary::GetRightVector(GetYawBasedRotator()));
	AddMovementInput(RightVector, Amount);
}

// Move to WC
void ABasePlayer::Attack()
{
	IsAttack = !IsAttack;
	UE_LOG(LogBasePlayer, Display, TEXT("Attack state: %s"), IsAttack ? *FString("true") : *FString("false"));
}

//Move to WC
void ABasePlayer::Equip()
{
	if (!CanEquip()) return;

	EquipInProgress = true;
	AllowMove(EMovementMode::MOVE_None);
	
	UseBattleMode(ChangeBattleMode());

	const auto CharacterMesh = GetMesh();
	if (!CharacterMesh) { return; }

	CurrentEquipState = (++CurrentEquipState) % EquippedStateAnimInstances.Num();
	if (EquippedStateAnimInstances[CurrentEquipState])
	{
		CharacterMesh->SetAnimInstanceClass(EquippedStateAnimInstances[CurrentEquipState]);
	}

	if (EquipAnimations[CurrentEquipAnimation])
	{
		PlayAnimMontage(EquipAnimations[CurrentEquipAnimation]);
		CurrentEquipAnimation = (++CurrentEquipAnimation) % EquipAnimations.Num();
	}
}

//Move to WC
bool ABasePlayer::CanEquip() const
{
	return !EquipInProgress && !GetMovementComponent()->IsFalling();
}

//Move to WC
void ABasePlayer::InitAnimNotifies()
{
	for (const auto EquipAnimation : EquipAnimations)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimation);
		if (!EquipFinishedNotify) continue;

		EquipFinishedNotify->OnNotified.AddUObject(this, &ABasePlayer::OnEquipFinished);
	}
}

//Move to WC
void ABasePlayer::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	const auto CharacterMesh = GetMesh();
	if (!CharacterMesh || CharacterMesh != MeshComp) { return; }

	EquipInProgress = false;
	AllowMove(EMovementMode::MOVE_Walking);
}

FRotator ABasePlayer::GetYawBasedRotator() const
{
	return FRotator(0.f, GetControlRotation().Yaw, 0.f);
}

//TO DO think about place
bool ABasePlayer::ChangeBattleMode()
{
	BattleMode = !BattleMode;
	return BattleMode;
}

//Make public???
void ABasePlayer::AllowMove(EMovementMode NewMovementMode) const
{
	const auto MovementComponent = GetCharacterMovement();
	if (!MovementComponent) return;
	
	MovementComponent->SetMovementMode(NewMovementMode);
}

//WC???
void ABasePlayer::UseBattleMode(const bool Mode)
{
	bUseControllerRotationYaw = Mode;
	GetCharacterMovement()->bOrientRotationToMovement = !Mode;
}

void ABasePlayer::OnDeath()
{
	AllowMove(MOVE_None);
	SetLifeSpan(5.f);

	//uncomment it after the collision will be set
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// remove after UI implemented
	HealthText->SetText(FText::FromString(FString::Printf(TEXT("%.0f"), HealthComponent->GetHealth())));
}

void ABasePlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ABasePlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ABasePlayer::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &ABasePlayer::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnAround", this, &ABasePlayer::AddControllerYawInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ABasePlayer::Jump);
	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &ABasePlayer::ChangeRunState);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &ABasePlayer::ChangeRunState);

	PlayerInputComponent->BindAction("Attack", IE_Pressed, this, &ABasePlayer::Attack);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &ABasePlayer::Equip);
}
