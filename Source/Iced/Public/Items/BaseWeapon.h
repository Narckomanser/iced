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

protected:
	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                           const FHitResult& SweepResult) override;
	
	virtual void OnTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation,
	                               UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	                               const UDamageType* DamageType, AActor* DamageCauser) override;


private:
	bool CanTakeDamage() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 10.f;
};
