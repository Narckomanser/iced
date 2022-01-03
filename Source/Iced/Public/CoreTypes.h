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

//TODO create ENUM to it
//TODO remade from struct to map where key is ENUM and VALUE in Anim
USTRUCT(BlueprintType)
struct FAnimList
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* AttackAnim;;
};