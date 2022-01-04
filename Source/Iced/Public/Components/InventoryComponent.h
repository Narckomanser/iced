// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Iced/Public/CoreTypes.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class ABaseItem;
class ABasePlayer;
class UWeaponComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	ABaseItem* GetEquippedWeapon() const { return EquippedWeapon; }

	void OnAttachItem(USkeletalMeshComponent* MeshComp);

protected:
	virtual void BeginPlay() override;

private:
	void GrabSubscriber();
	void Eqiup(ABaseItem* NewWeapon);
	void DropEqippedWeapon(ABasePlayer* Owner, const UWeaponComponent* WeaponComponent);
	void AttachItemToSocket(USkeletalMeshComponent* MeshComp, const FName SocketName) const;
	void InitNotifies(const TArray<UAnimMontage*>& AnimList);
	void RemoveNotifies(const TArray<UAnimMontage*>& AnimList) const;
	void SetupEquippedItem(ABasePlayer* ItemOwner, bool ShouldIgnore);

private:
	UPROPERTY()
	ABaseItem* EquippedWeapon = nullptr;
};
