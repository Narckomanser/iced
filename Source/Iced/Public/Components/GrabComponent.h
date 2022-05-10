// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Iced/Public/CoreTypes.h"
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

	ABaseItem* DetectItem() const;

protected:
	virtual void BeginPlay() override;

private:
	void SetupPlayerInputComponent();

	FVector GetStartPoint() const;
	FVector GetEndPoint() const;

	void Grab();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Grabber setup")
	FName BoneName = "head";

	UPROPERTY(EditDefaultsOnly, Category = "Grabber setup")
	float GrabDistance = 200.f;
};
