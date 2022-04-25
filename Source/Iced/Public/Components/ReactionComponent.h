// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ReactionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ICED_API UReactionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UReactionComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UAnimMontage* GetResponseAnimation(const UDamageType* DamageType);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Response")
	TMap<TSubclassOf<UDamageType>, UAnimMontage*> ResponseList;
};
