// Narckomanser's game


#include "Items/BaseWeapon.h"

#include "Components/CapsuleComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

ABaseWeapon::ABaseWeapon()
{
	HitComponent = CreateDefaultSubobject<UCapsuleComponent>("Hit Component");
	HitComponent->SetupAttachment(GetRootComponent());
}

//TODO add sweep results to set. replace OverlapTimerDelay to AnimEnd. After timer end - clear the set
void ABaseWeapon::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanTakeDamage()) { return; }
	
	if (const auto ItemOwner = GetOwner<APawn>())
	{
		//TODO calculate damage with some modifiers
		OtherActor->TakeDamage(DamageAmount, FPointDamageEvent{}, ItemOwner->GetController(), GetOwner());
		GetWorld()->GetTimerManager().SetTimer(OverlapTimer, OverlapTimerDelay, false);

		UE_LOG(LogBaseWeapon, Display, TEXT("%s's %s hitted to: %s | %s"), *this->GetName(), *OverlappedComponent->GetName(), *OtherActor->GetName(), *OtherComp->GetName());
	}
}

void ABaseWeapon::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType,
	AActor* DamageCauser)
{
}

bool ABaseWeapon::CanTakeDamage() const
{
	return bDoesInCombat && (GetWorld()->GetTimerManager().GetTimerRemaining(OverlapTimer) <= 0.f);

}
