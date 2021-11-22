// Narckomanser's game

#include "Components/HealthComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogHealthComponent, All, All);

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();
	SetHealth(MaxHealth);

	AActor* Owner = GetOwner();
	if (Owner)
	{
		Owner->OnTakeRadialDamage.AddDynamic(this, &UHealthComponent::OnTakeRadialDamage);
	}


	UE_LOG(LogHealthComponent, Display, TEXT("Current Health: %f"), CurrentHealth);
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

void UHealthComponent::ApplyDamage(float Damage)
{
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