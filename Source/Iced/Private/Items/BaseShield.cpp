// Narckomanser's game


#include "Items/BaseShield.h"

#include "BasePlayer.h"
#include "Components/BoxComponent.h"

DEFINE_LOG_CATEGORY_STATIC(LogBaseShield, All, All)

ABaseShield::ABaseShield()
{
	HitComponent = CreateDefaultSubobject<UBoxComponent>("Hit Component");
	HitComponent->SetupAttachment(GetRootComponent());
}

void ABaseShield::OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                                const FHitResult& SweepResult)
{
}

void ABaseShield::OnTakePointDamageHandle(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                                    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                    const UDamageType* DamageType,
                                    AActor* DamageCauser)
{
	if (const auto ItemOwner = DamagedActor->GetOwner<ABasePlayer>())
	{
		const float NewDamage = Damage - ProtectionAmount;
		
		FPointDamageEvent DamageEvent;
		DamageEvent.DamageTypeClass = DamageType->GetClass();
		
		ItemOwner->TakeDamage(NewDamage, DamageEvent, InstigatedBy, this);
	}
}

void ABaseShield::OnAttackHandle(const EAttackTypes AttackType)
{
}
