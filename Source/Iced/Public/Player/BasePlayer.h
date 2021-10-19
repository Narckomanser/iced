// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ICED_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ABasePlayer(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsRunning() const { return IsRun; }

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
	UCameraComponent* CameraComponent;

private:
	void MoveForward(const float Amount);
	void MoveRight(const float Amount);
	void Attack();
	void Equip();

	void InitAnimNotifies();

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);

	FRotator GetYawBasedRotator() const;

	void ChangeRunState() { IsRun = !IsRun; }
	void ChangeEquipState() { EquipInProgress = !EquipInProgress; }

private:
	bool IsRun = false;
	bool IsAttack = false;
	bool EquipInProgress = false;

	int32 CurrentEquipAnimation = 0;

	UPROPERTY(EditDefaultsOnly, Category = "Equipped State Animations")
	TArray<TSubclassOf<UAnimInstance>> EquippedStateAnimationBluePrints;

	UPROPERTY(EditDefaultsOnly, Category = "Equip Animation")
	TArray<UAnimMontage*> EquipAnimations;
};
