// Narckomanser's game

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Iced/Public/CoreTypes.h"
#include "WeaponComponent.generated.h"

//TODO unite Current* variables and setup EquipData

class ABaseItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UWeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UWeaponComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	//TODO return anim by Enum(check CoreTypes)
	UAnimMontage* GetAnim() const { return CombatAnimList.AttackAnim; }

	int8 GetCurrentStanceState() const { return CurrentStanceState; }
	FName GetStanceSocketName() const { return EquipData[CurrentStanceState].EquipSocketName; }

protected:
	virtual void BeginPlay() override;

private:
	void SetupPlayerInputComponent();
	void Attack();
	void InitAnimNotifies();
	void ChangeStance();
	bool CanChangeStance() const;
	void OnStanceChanged(USkeletalMeshComponent* MeshComp);
	bool ChangeBattleMode();
	void UseBattleMode(const bool Mode) const;
	void WeaponOverlapEventEnabler() const;

private:
	bool EquipInProgress = false;
	bool BattleMode = false;
	int8 CurrentStanceState = 0;
	int8 CurrentEquipState = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped State Animations")
	TArray<TSubclassOf<UAnimInstance>> EquippedStateAnimInstances;

	UPROPERTY(EditDefaultsOnly, Category = "Anim Data")
	FAnimList CombatAnimList;

	UPROPERTY(EditDefaultsOnly, Category = "Equip Data")
	TArray<FEquipData> EquipData;

	FTimerHandle OverlapEnableTimer;
};
