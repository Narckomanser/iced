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

protected:
	virtual void BeginPlay() override;

private:
	void SetupPlayerInputComponent();
	
	void Attack();
	void ChangeStance();

	void GrabSubscriber();
	
	bool CanEquip() const;
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnAttachItem(USkeletalMeshComponent* MeshComp);

	bool ChangeBattleMode();
	void UseBattleMode(const bool Mode) const;

	void InitAnimNotifies();

	void Eqiup(AActor* NewWeapon);
	void DropEqippedWeapon();
	void AttachItemToSocket();

private:
	UPROPERTY()
	UInputComponent* InputComponent;
	
	bool EquipInProgress = false;
	bool BattleMode = false;

	int8 CurrentEquipAnimation = 0;
	int8 CurrentEquipState = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped State Animations")
	TArray<TSubclassOf<UAnimInstance>> EquippedStateAnimInstances;

	UPROPERTY(EditDefaultsOnly, Category = "Equip Data")
	TArray<FEquipData> EquipData;

	UPROPERTY(EditDefaultsOnly, Category = "Anim Data")
	FAnimList CombatAnimList;

	UPROPERTY()
	ABaseItem* EquippedWeapon = nullptr;
};
