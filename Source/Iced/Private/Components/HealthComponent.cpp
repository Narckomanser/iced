// Narckomanser's game

#include "Components/HealthComponent.h"

#include "BasePlayer.h"
#include "ReactionComponent.h"
#include "GameFramework/Character.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);

	if (const auto Owner = GetOwner())
	{
		Owner->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::OnTakeRadialDamage);
		Owner->OnTakePointDamage.AddDynamic(this, &UHealthComponent::OnTakePointDamage);
	}
}

void UHealthComponent::SetHealth(const float NewHealth)
{
	CurrentHealth = FMath::Clamp(NewHealth, 0.f, MaxHealth);
}

void UHealthComponent::OnTakeRadialDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                          FVector Origin, FHitResult HitInfo, AController* InstigatedBy,
                                          AActor* DamageCauser)
{
	ApplyDamage(Damage);
}

void UHealthComponent::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy,
                                         FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName,
                                         FVector ShotFromDirection,
                                         const UDamageType* DamageType, AActor* DamageCauser)
{
	ApplyDamage(Damage);
	UE_LOG(LogHealthComponent, Display, TEXT("Damage Dealer: %s, Damage Type %s"), *DamageCauser->GetName(),
	       *DamageType->GetClass()->GetName());
	
	if (const auto ReactionComponent = GetOwner<ABasePlayer>()->GetReactionComponent(); ReactionComponent)
	{
		GetOwner<ACharacter>()->PlayAnimMontage(ReactionComponent->GetResponseAnimation(DamageType));
	}
}

void UHealthComponent::OnTakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                       AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogHealthComponent, Display, TEXT("OnTakeAnyDamage"));
	//ApplyDamage(Damage);
}

void UHealthComponent::ApplyDamage(float Damage)
{
	//TODO think about damage calculation
	if (Damage <= 0.f || IsDead()) return;

	SetHealth(CurrentHealth - Damage);

	if (IsDead())
	{
		OnDeath.Broadcast();
	}
}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
