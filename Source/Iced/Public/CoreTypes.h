// Narckomanser's game

#pragma once

#include "CoreTypes.generated.h"

class ABaseItem;

USTRUCT(BlueprintType)
struct FStanceData
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Stance", meta=(DisplayName = "Stance Anim Blueprint"))
	TSubclassOf<UAnimInstance> StanceAnimInstance;
	
	UPROPERTY(EditDefaultsOnly, Category = "Stance", meta=(DisplayName = "Transition Animation"))
	UAnimMontage* TransitionAnimation;

	UPROPERTY(EditDefaultsOnly, Category = "Stance", meta=(DisplayName = "Stance Socket Name"))
	FName WeaponSocket;
};


UENUM()
enum class EAttackTypes
{
	DefaultAttack UMETA(DisplayName = "Default Attack"),
	MightAttack UMETA(DisplayName = "Might Attack"),

	Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EAttackTypes, EAttackTypes::Count)


UENUM()
enum class EItemTypes
{
	Weapon,
	Shield,
	
	Count UMETA(Hidden)
};

ENUM_RANGE_BY_COUNT(EItemTypes, EItemTypes::Count)

UENUM()
enum EAttributes
{
	
};


DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrabItem, ABaseItem*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttack, EAttackTypes);