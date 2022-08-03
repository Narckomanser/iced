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

USTRUCT(BlueprintType)
struct FLevelData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Level", meta=(DisplayName = "Level"))
	uint8 AttributeLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Attribute Level", meta=(DisplayName = "Experience"))
	uint8 AttributeExperience;

	//index equals curr lvl
	//value equals needed exp to next lvl
	UPROPERTY(EditDefaultsOnly, Category = "Attribute Level", meta=(DisplayName = "Exp List To Next Lvl"))
	TArray<uint8> ExpToLvlUp;

	//TODO remove after UI impl
	FString ToString() const { return FString::Printf(TEXT("Level: %i, Exp: %i"), AttributeLevel, AttributeExperience); }
};


DECLARE_MULTICAST_DELEGATE(FOnDeathSignature);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGrabItemSignature, ABaseItem*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttackSignature, EAttackTypes);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnExperienceUpSignature, EAttributes, AActor*);