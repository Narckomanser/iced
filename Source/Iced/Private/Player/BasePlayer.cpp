#include "Player/BasePlayer.h"

#include "GrabComponent.h"
#include "PlayerMovementComponent.h"
#include "HealthComponent.h"
#include "WeaponComponent.h"
#include "BaseItem.h"
#include "InventoryComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/TextRenderComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"


DEFINE_LOG_CATEGORY_STATIC(LogBasePlayer, All, All);

ABasePlayer::ABasePlayer(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(
		ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = true;

	//TODO setup collision!

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArmComponent");
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->SetupAttachment(SpringArmComponent);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("HealthComponent");
	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>("WeaponComponent");
	GrabComponent = CreateDefaultSubobject<UGrabComponent>("GrabComponent");
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("InventoryComponent");

	// TODO remove after UI implemented
	HealthText = CreateDefaultSubobject<UTextRenderComponent>("Health Text");
	HealthText->SetupAttachment(GetRootComponent());
}

void ABasePlayer::AllowMove(EMovementMode NewMovementMode) const
{
	const auto MovementComponent = GetCharacterMovement();
	if (!MovementComponent) return;

	MovementComponent->SetMovementMode(NewMovementMode);
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

FRotator ABasePlayer::GetYawBasedRotator() const
{
	return FRotator(0.f, GetControlRotation().Yaw, 0.f);
}


void ABasePlayer::OnDeath()
{
	AllowMove(MOVE_None);
	SetLifeSpan(5.f);

	// TODO setup collision and uncomment line
	//GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECR_Ignore);

	GetMesh()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	GetMesh()->SetSimulatePhysics(true);
}

void ABasePlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// TODO remove after UI implemented
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
}
