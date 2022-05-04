// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Iced/Public/CoreTypes.h"
#include "AttributeComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UAttributeComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	uint8 GetAttribute(const EAttributes Attribute) const { return AttributesLevel[Attribute]; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Attributes")
	TMap<EAttributes, uint8> AttributesLevel;

	// hold current attr level
	// hold current exp / or just send delegates from components and increments lvl after achievements certain value 
	// hold needed exp to up level / how to hold it??? or calculate it dynamically
	// add exp by delegate from components
	// compare current and needed when add exp
	
};
