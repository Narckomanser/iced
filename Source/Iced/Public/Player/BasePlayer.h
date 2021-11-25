// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;
class UWeaponComponent;
class UGrabComponent;
class ABaseItem;

UCLASS()
class ICED_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ABasePlayer(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsRunning() const { return IsRun; }

	void AllowMove(EMovementMode NewMovementMode) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	void Grab();

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UTextRenderComponent* HealthText;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UWeaponComponent* WeaponComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGrabComponent* GrabComponent;

private:
	void MoveForward(const float Amount);
	void MoveRight(const float Amount);
	FRotator GetYawBasedRotator() const;

	void ChangeRunState() { IsRun = !IsRun; }

	UFUNCTION()
	void OnDeath();

	// TODO remove or call WC same methods
	void Attack();
	void Equip();

private:
	bool IsRun = false;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Arm Socket")
	FName ArmSocketName = "beltWeapon";
	
UPROPERTY(EditDefaultsOnly, Category = "Weapon Belt Socket")
	FName BeltSocketName = "hand_rSocket";

	UPROPERTY()
	AActor* EquippedWeapon = nullptr;
};
