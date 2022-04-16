// Narckomanser's game


#include "Items/BaseShield.h"

#include "BasePlayer.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseShield, All, All)

void ABaseShield::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult)
{
}

void ABaseShield::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                                    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                    const UDamageType* DamageType,
                                    AActor* DamageCauser)
{
	UE_LOG(LogBaseShield, Display, TEXT("OnTakePointDamage in shield"));
	
	const auto ItemOwner = GetOwner<ABasePlayer>();
	if (ItemOwner)
	{
		UE_LOG(LogBaseShield, Display, TEXT("Shield Owner: %s"), *ItemOwner->GetName());
		const float NewDamage = Damage - ProtectionAmount;
		
		ItemOwner->TakeDamage(NewDamage, FPointDamageEvent{}, InstigatedBy, DamageCauser);
	}
}
