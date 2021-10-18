#include "Player/BasePlayer.h"

#include "PlayerMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

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

FRotator ABasePlayer::GetYawBasedRotator() const
{
	return FRotator(0.f, GetControlRotation().Yaw, 0.f);
}

void ABasePlayer::Attack()
{
	IsAttack = !IsAttack;
	UE_LOG(LogBasePlayer, Display, TEXT("Attack state: %s"), IsAttack ? *FString("true") : *FString("false"));
}

void ABasePlayer::Equip()
{
	CurrentEquipAnimation = (CurrentEquipAnimation + 1) % EquipAnimations.Num();
	PlayAnimMontage(EquipAnimations[CurrentEquipAnimation]);
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
