// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Iced/Public/CoreTypes.h"
#include "BaseItem.generated.h"

class UCapsuleComponent;

UCLASS()
class ICED_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                           const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnTakePointDamageHandle(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
	                               FVector HitLocation,
	                               class UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection,
	                               const class UDamageType* DamageType, AActor* DamageCauser);

	virtual void OnAttackHandle(const EAttackTypes AttackType);

	UPrimitiveComponent* GetHitComponent() const { return HitComponent; }
	UStaticMeshComponent* GetMesh() const { return ItemMesh; }

	void ChangeCombatState();
	bool IsInCombatState() const { return bDoesInCombat; }

	EItemTypes GetItemType() const { return ItemType; }
	FName GetItemSocket() const { return Socket; }


protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* RootCapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPrimitiveComponent* HitComponent = nullptr;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Item Durability")
	float Durability = 100;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Type")
	EItemTypes ItemType;

	UPROPERTY(EditDefaultsOnly, Category = "Item Socket")
	FName Socket;

	UPROPERTY(EditDefaultsOnly)
	float OverlapTimerDelay = 1.f;

	FTimerHandle OverlapTimer;

	bool bDoesInCombat = false;
};
