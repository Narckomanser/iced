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

	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                           const FHitResult& SweepResult) override;

private:
	bool CanTakeDamage() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 10.f;
};
