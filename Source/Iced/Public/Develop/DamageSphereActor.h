// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamageSphereActor.generated.h"

UCLASS()
class ICED_API ADamageSphereActor : public AActor
{
	GENERATED_BODY()

public:
	ADamageSphereActor();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, Category = "SphereParamater")
	float Radius = 50.f;

	UPROPERTY(EditAnywhere, Category = "SphereParamater")
	FColor Color = FColor::Red;

	UPROPERTY(EditAnywhere, Category = "DamageParamater")
	float Damage = 1.f;

	UPROPERTY(EditAnywhere, Category = "DamageParamater")
	bool DoFullDamage = true;
};
