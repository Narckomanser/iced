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
	void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                   const FHitResult& SweepResult);
	
	UCapsuleComponent* GetHitCapsuleComponent() const { return HitCapsuleComponent; }
	UStaticMeshComponent* GetMesh() const { return ItemMesh; }

	//TODO add mesh condition
	void ChangeAttackState(USkeletalMeshComponent* MeshComp) { DoesInAttack = !DoesInAttack; }
	bool CanAttack() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* RootCapsuleComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* HitCapsuleComponent;

private:
	UPROPERTY(EditDefaultsOnly, Category = "Damage")
	float DamageAmount = 10.f;

	FTimerHandle OverlapTimer;

	UPROPERTY(EditDefaultsOnly)
	float OverlapTimerDelay = 1.f;

	bool DoesInAttack = false;
};
