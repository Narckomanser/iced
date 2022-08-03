// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "Iced/Public/CoreTypes.h"
#include "BasePlayerState.generated.h"


UCLASS()
class ICED_API ABasePlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	FLevelData& GetLevelData(const EAttributes& Attribute) { return SkillLevels[Attribute]; }

private:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TMap<EAttributes, FLevelData> SkillLevels;
};
