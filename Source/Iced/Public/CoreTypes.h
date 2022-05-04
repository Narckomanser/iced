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
enum class EAttributes
{
	Strength,
	Agility,
	Constitution,
	Wisdom,

	Count UMETA(Hidden)
};

//TODO for AttributeComponent / check comments in .h file
USTRUCT(BlueprintType)
struct FAttributeLevel
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Level", meta=(DisplayName = "Attribute"))
	uint8 CurrentLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Level", meta=(DisplayName = "Experience To Up"))
	TArray<float> ExperienceToNextLevel;
};


DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrabItem, ABaseItem*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttack, EAttackTypes);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnLevelUp, EAttributes, AActor*);