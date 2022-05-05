// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Iced/Public/CoreTypes.h"
#include "ProgressComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UProgressComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UProgressComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;


protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TMap<EAttributes, FLevelData> AttributesLevel;

	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TMap<EAttributes, float> StepToBroadcastExp;
	
	// add exp by delegate from components
	// compare current and needed when add exp



	//array where indexes == lvl
	//vals == uint8
	//every n meters component send event and totalexp++
	//after totalexp update check array[currLVL] == totalexp and up lvl
};
