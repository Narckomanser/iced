// Narckomanser's game


#include "Items/BaseWeapon.h"

void ABaseWeapon::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!CanTakeDamage()) { return; }
	
	if (const auto ItemOwner = GetOwner<APawn>())
	{
		//TODO calculate damage with some modifiers
		OtherActor->TakeDamage(DamageAmount, FPointDamageEvent{}, ItemOwner->GetController(), this);
		
		GetWorld()->GetTimerManager().SetTimer(OverlapTimer, OverlapTimerDelay, false);
	}
}

bool ABaseWeapon::CanTakeDamage() const
{
	return bDoesInCombat && (GetWorld()->GetTimerManager().GetTimerRemaining(OverlapTimer) <= 0.f);

}
