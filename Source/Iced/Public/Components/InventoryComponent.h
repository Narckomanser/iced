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
	void DropItem(const EItemTypes ItemType);
	void AttachItemToSocket(ABaseItem* Item, const FName SocketName, USkeletalMeshComponent* MeshComp) const;
	void SetupItem(ABaseItem* Item, AActor* NewOwner);
	void SetupItemCollision(const ABaseItem* Item, ABasePlayer* ItemOwner, const bool ShouldIgnore);
	void AddToEquipment(const EItemTypes ItemType, ABaseItem* NewItem);

	void InitEquipmentMap();

	void DevSpawnItems();

private:
	UPROPERTY()
	TMap<EItemTypes, ABaseItem*> Equipment;

	UPROPERTY(EditDefaultsOnly, Category = "Start Items")
	TArray<TSubclassOf<ABaseItem>> DevBaseInventory;
};
