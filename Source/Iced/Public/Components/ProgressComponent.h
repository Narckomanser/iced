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
	void FOnExperienceUpHandler(EAttributes Attribute, AActor* AttributeOwner);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TMap<EAttributes, FLevelData> AttributesData;
};
