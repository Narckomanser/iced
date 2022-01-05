// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Items/BaseItem.h"
#include "BaseShield.generated.h"

UCLASS()
class ICED_API ABaseShield : public ABaseItem
{
	GENERATED_BODY()

public:
	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                           const FHitResult& SweepResult) override;
};
