// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GrabComponent.generated.h"

class ABaseItem;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UGrabComponent : public UActorComponent
{
	GENERATED_BODY()
public:
	UGrabComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	FHitResult DetectItem() const;

protected:
	virtual void BeginPlay() override;

private:
	FVector GetStartPoint() const;
	FVector GetEndPoint() const;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Start Distance Bone")
	FName BoneName = "head";

	UPROPERTY(EditDefaultsOnly, Category = "Grab Distance")
	float GrabDistance = 200.f;
};
