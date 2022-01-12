// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Iced/Public/CoreTypes.h"
#include "InventoryComponent.generated.h"

class ABaseItem;
class ABasePlayer;
class UCombatComponent;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UInventoryComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	ABaseItem* GetEquippedItem(EItemTypes ItemType) const { return Equipment[ItemType]; }

	void OnAttachItem(USkeletalMeshComponent* MeshComp);

protected:
	virtual void BeginPlay() override;

private:
	void GrabSubscriber();
	void Eqiup(ABaseItem* NewItem);
	void DropItem(EItemTypes ItemType, ABasePlayer* Owner, const UCombatComponent* CombatComponent);
	void AttachItemToSocket(ABaseItem* Item, const FName SocketName, USkeletalMeshComponent* MeshComp) const;
	void InitNotifies(const TArray<UAnimMontage*>& AnimList);
	void RemoveNotifies(const TArray<UAnimMontage*>& AnimList) const;
	void SetupEquippedItem(ABaseItem* Item, ABasePlayer* ItemOwner, bool ShouldIgnore);

	void InitEquipmentList();

private:
	UPROPERTY()
	TMap<EItemTypes, ABaseItem*> Equipment;
};
