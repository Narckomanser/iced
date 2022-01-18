// Narckomanser's game


#include "Items/BaseWeapon.h"

#include "PhysicalMaterials/PhysicalMaterial.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseWeapon, All, All)

//TODO add sweep results to set. replace OverlapTimerDelay to AnimEnd. After timer end - clear the set
void ABaseWeapon::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanTakeDamage()) { return; }
	
	if (const auto ItemOwner = GetOwner<APawn>())
	{
		//TODO calculate damage with some modifiers
		OtherActor->TakeDamage(DamageAmount, FPointDamageEvent{}, ItemOwner->GetController(), this);
		GetWorld()->GetTimerManager().SetTimer(OverlapTimer, OverlapTimerDelay, false);

		UE_LOG(LogBaseWeapon, Display, TEXT("%s"), *SweepResult.ToString());
	}
}

bool ABaseWeapon::CanTakeDamage() const
{
	return bDoesInCombat && (GetWorld()->GetTimerManager().GetTimerRemaining(OverlapTimer) <= 0.f);

}
