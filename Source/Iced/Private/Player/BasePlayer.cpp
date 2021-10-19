#include "Player/BasePlayer.h"

#include "PlayerMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Notifies/EquipFinishedAnimNotify.h"
#include "Notifies/NotifyUtils.h"

DEFINE_LOG_CATEGORY_STATIC(LogBasePlayer, All, All);

ABasePlayer::ABasePlayer(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	bUseControllerRotationYaw = false;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void ABasePlayer::BeginPlay()
{
	Super::BeginPlay();
	
	InitAnimNotifies();
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


void ABasePlayer::Attack()
{
	IsAttack = !IsAttack;
	UE_LOG(LogBasePlayer, Display, TEXT("Attack state: %s"), IsAttack ? *FString("true") : *FString("false"));
}

void ABasePlayer::Equip()
{
	ChangeEquipState();
	UE_LOG(LogBasePlayer, Display, TEXT("Equip In Progress"));
	
	GetController()->SetIgnoreMoveInput(true);
	
	CurrentEquipAnimation = (CurrentEquipAnimation + 1) % EquipAnimations.Num();
	PlayAnimMontage(EquipAnimations[CurrentEquipAnimation]);
}

void ABasePlayer::InitAnimNotifies()
{
	for (const auto EquipAnimation : EquipAnimations)
	{
		const auto EquipFinishedNotify = FNotifyUtils::FindNotifyByClass<UEquipFinishedAnimNotify>(EquipAnimation);
		if (!EquipFinishedNotify) continue;
		
		EquipFinishedNotify->OnNotified.AddUObject(this, &ABasePlayer::OnEquipFinished);
	}
}

void ABasePlayer::OnEquipFinished(USkeletalMeshComponent* MeshComp)
{
	if (GetMesh() != MeshComp) { return; }
	ChangeEquipState();

	GetController()->SetIgnoreMoveInput(false);

	UE_LOG(LogBasePlayer, Display, TEXT("Equip Finished"));
}

FRotator ABasePlayer::GetYawBasedRotator() const
{
	return FRotator(0.f, GetControlRotation().Yaw, 0.f);
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
