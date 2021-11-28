﻿// Narckomanser's game

#pragma once

#include "CoreTypes.generated.h"

//TODO move all delegates here

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

USTRUCT(BlueprintType)
struct FAnimList
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Anim")
	UAnimMontage* AttackAnim;;
};