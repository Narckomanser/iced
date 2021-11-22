// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnDealthSignature);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UHealthComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "Health Component")
	float GetHealth() const { return CurrentHealth; }

	UFUNCTION()
	bool IsDead() const { return FMath::IsNearlyZero(CurrentHealth); }

public:
	FOnDealthSignature OnDeath;

protected:
	virtual void BeginPlay() override;

private:
	void SetHealth(const float NewHealth);

	UFUNCTION()
	void OnTakeRadialDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, FVector Origin,
	                        FHitResult HitInfo, class AController* InstigatedBy, AActor* DamageCauser);

	void ApplyDamage(float Damage);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Health", meta = (ClampMin = 0.f, ClampMax = 1000))
	;
	float MaxHealth = 100.f;

	float CurrentHealth;
};