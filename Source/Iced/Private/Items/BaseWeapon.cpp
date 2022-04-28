// Narckomanser's game


#include "Items/BaseWeapon.h"

#include "BasePlayer.h"
#include "CombatComponent.h"
#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ABaseWeapon::ABaseWeapon()
{
	HitComponent = CreateDefaultSubobject<UCapsuleComponent>("Hit Component");
	HitComponent->SetupAttachment(GetRootComponent());
	
	DamageTypesInit();
}

void ABaseWeapon::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanAttack()) { return; }
	
	if (const auto ItemOwner = GetOwner<APawn>(); CanDealDamage(ItemOwner, OtherActor))
	{
		//TODO calculate damage with some modifiers

		FPointDamageEvent DamageEvent;
		DamageEvent.DamageTypeClass = DamageTypes[LastAttackType];
		
		OtherActor->TakeDamage(DamageAmount, DamageEvent, ItemOwner->GetController(), GetOwner());
		GetWorld()->GetTimerManager().SetTimer(OverlapTimer, OverlapTimerDelay, false);

		UE_LOG(LogBaseWeapon, Display, TEXT("%s's %s hitted to: %s | %s | %s"), *this->GetName(), *OverlappedComponent->GetName(), *OtherActor->GetOwner()->GetName(), *OtherActor->GetName(), *OtherComp->GetName());
	}
}

void ABaseWeapon::OnTakePointDamageHandle(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
	AActor* DamageCauser)
{
}

bool ABaseWeapon::CanAttack() const
{
	return bDoesInCombat && (GetWorld()->GetTimerManager().GetTimerRemaining(OverlapTimer) <= 0.f);

}

bool ABaseWeapon::CanDealDamage(const AActor* DamageDealer, const AActor* DamageTaker) const
{
	return DamageDealer != DamageTaker && DamageDealer != DamageTaker->GetOwner();
}

void ABaseWeapon::DamageTypesInit()
{
	for (EAttackTypes AttackType : TEnumRange<EAttackTypes>())
	{
		DamageTypes.Add(AttackType, nullptr);
	}
}

void ABaseWeapon::OnAttackHandle(const EAttackTypes AttackType)
{
	LastAttackType = AttackType;
}
