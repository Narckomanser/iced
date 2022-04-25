// Narckomanser's game


#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "BaseWeapon.generated.h"

UCLASS()
class ICED_API ABaseWeapon : public ABaseItem
{
	GENERATED_BODY()

public:
	ABaseWeapon();

	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                           const FHitResult& SweepResult) override;

	virtual void OnTakePointDamageHandle(AActor* DamagedActor, float Damage, AController* InstigatedBy,
	                                     FVector HitLocation,
	                                     UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	                                     const UDamageType* DamageType, AActor* DamageCauser) override;

	virtual void OnAttackHandle(const EAttackTypes AttackType) override;
	
private:
	bool CanAttack() const;
	bool CanDealDamage(const AActor* DamageDealer, const AActor* DamageTaker) const;

	void DamageTypesInit();


private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	TMap<EAttackTypes, TSubclassOf<UDamageType>> DamageTypes;

	EAttackTypes LastAttackType;
};
