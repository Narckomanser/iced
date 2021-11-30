// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

class UCapsuleComponent;

UCLASS()
class ICED_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	ABaseItem();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnComponentHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UCapsuleComponent* GetCollisionComponent() const { return CapsuleComponent; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* CapsuleComponent;
};
