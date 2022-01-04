// Narckomanser's game

#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Iced/Public/CoreTypes.h"
#include "WeaponComponent.generated.h"

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
	TArray<UAnimMontage*> GetAnimList() const;

	uint8 GetCurrentStanceState() const { return CurrentStanceState; }
	FName GetStanceSocketName() const { return StanceData[CurrentStanceState].StanceSocketName; }

protected:
	virtual void BeginPlay() override;

private:
	void SetupPlayerInputComponent();
	void Attack();
	void InitAnimNotifies();
	void ChangeStance();
	bool CheckCalmState() const;
	void OnStanceChanged(USkeletalMeshComponent* MeshComp);
	bool ChangeBattleMode();
	void UseBattleMode(const bool bMode) const;
	void WeaponOverlapEventEnabler() const;

	void InitCombatAnimList();

private:
	bool bDoesEquipInProgress = false;
	bool bBattleMode = false;
	int8 CurrentStanceState = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Anim Data")
	TMap<EAttackTypes, UAnimMontage*> CombatAnimList;

	UPROPERTY(EditDefaultsOnly, Category = "Stance Data")
	TArray<FStanceData> StanceData;

	FTimerHandle OverlapEnableTimer;
};
