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

void ABaseShield::OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
                                    UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
                                    const UDamageType* DamageType,
                                    AActor* DamageCauser)
{
	UE_LOG(LogBaseShield, Display, TEXT("OnTakePointDamage in shield"));
	
	if (const auto ItemOwner = DamagedActor->GetOwner<ABasePlayer>())
	{
		UE_LOG(LogBaseShield, Display, TEXT("Shield Owner: %s"), *ItemOwner->GetName());
		const float NewDamage = Damage - ProtectionAmount;
		
		ItemOwner->TakeDamage(NewDamage, FPointDamageEvent{}, InstigatedBy, DamageCauser);
	}
}
