// Narckomanser's game

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Iced/Public/CoreTypes.h"
#include "WeaponComponent.generated.h"

//TODO unite Current* variables and setup EquipData

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void Attack();
	void Equip();

protected:
	virtual void BeginPlay() override;

private:
	bool CanEquip() const;
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);

	bool ChangeBattleMode();
	void UseBattleMode(const bool Mode) const;

	void InitAnimNotifies();

	void EqiupWeapon(AActor* NewWeapon);

private:
	bool EquipInProgress = false;
	bool BattleMode = false;

	int8 CurrentEquipAnimation = 0;
	int8 CurrentEquipState = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped State Animations")
	TArray<TSubclassOf<UAnimInstance>> EquippedStateAnimInstances;

	UPROPERTY(EditDefaultsOnly, Category = "Equip Data")
	TArray<FEquipData> EquipData;

	UPROPERTY()
	AActor* EquippedWeapon = nullptr;
};
