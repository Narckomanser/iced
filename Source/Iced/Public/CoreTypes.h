// Narckomanser's game

#pragma once

#include "CoreTypes.generated.h"

class ABaseItem;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrabItem, ABaseItem*);

USTRUCT(BlueprintType)
struct FEquipData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Equip")
	UAnimMontage* TransitionAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Socket")
	FName EquipSocketName;

	//TODO may be need to move here AnimBPs???
};

UENUM()
enum class EAttackTypes
{
	DefaultAttack UMETA(DisplayName = "DefaultAttack"),
	MightAttack UMETA(DisplayName = "MightAttack"),

	Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EAttackTypes, EAttackTypes::Count)