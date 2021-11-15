// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;

UCLASS()
class ICED_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ABasePlayer(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsRunning() const { return IsRun; }

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UTextRenderComponent* HealthText;
private:
	void MoveForward(const float Amount);
	void MoveRight(const float Amount);
	void Attack();
	void Equip();

	bool CanEquip() const;

	void InitAnimNotifies();

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);

	FRotator GetYawBasedRotator() const;

	void ChangeRunState() { IsRun = !IsRun; }
	bool ChangeBattleMode();
	void AllowMove(EMovementMode NewMovementMode) const;
	void UseBattleMode(const bool Mode);

	UFUNCTION()
	void OnDeath();

private:
	bool IsRun = false;
	bool IsAttack = false;
	bool EquipInProgress = false;
	bool BattleMode = false;

	int32 CurrentEquipAnimation = 0;
	int32 CurrentEquipState = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped State Animations")
	TArray<TSubclassOf<UAnimInstance>> EquippedStateAnimInstances;

	UPROPERTY(EditDefaultsOnly, Category = "Equip Animation")
	TArray<UAnimMontage*> EquipAnimations;
};
