// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "BaseShield.generated.h"

UCLASS()
class ICED_API ABaseShield : public ABaseItem
{
	GENERATED_BODY()

public:
	ABaseShield();

	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                           const FHitResult& SweepResult) override;

	virtual void OnTakePointDamageHandle(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	                                     FVector HitLocation,
	                                     UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	                                     const UDamageType* DamageType, AActor* DamageCauser) override;

	virtual void OnAttackHandle(const EAttackTypes AttackType) override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Protection")
	float ProtectionAmount = 5;
};
