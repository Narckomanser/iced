// Narckomanser's game

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Iced/Public/CoreTypes.h"
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
	virtual void OnComponentBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                                           const FHitResult& SweepResult);

	UCapsuleComponent* GetHitCapsuleComponent() const { return HitCapsuleComponent; }
	UStaticMeshComponent* GetMesh() const { return ItemMesh; }

	void ChangeCombatState(USkeletalMeshComponent* MeshComp);
	bool IsInCombatState() const { return bDoesInCombat; }

	EItemTypes GetItemType() const { return ItemType; }
	FName GetItemSocket() const { return Socket; }


protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* RootCapsuleComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCapsuleComponent* HitCapsuleComponent;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Type")
	EItemTypes ItemType;

	UPROPERTY(EditDefaultsOnly, Category = "Item Socket")
	FName Socket;

	UPROPERTY(EditDefaultsOnly)
	float OverlapTimerDelay = 1.f;

	FTimerHandle OverlapTimer;

	bool bDoesInCombat = false;
};
