// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "BasePlayer.generated.h"

class USpringArmComponent;
class UCameraComponent;
class UHealthComponent;
class UTextRenderComponent;
class UCombatComponent;
class UGrabComponent;
class UInventoryComponent;
class ABaseItem;

UCLASS()
class ICED_API ABasePlayer : public ACharacter
{
	GENERATED_BODY()

public:
	ABasePlayer(const FObjectInitializer& ObjectInitializer);

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool IsRunning() const { return bIsRun; }

	void AllowMove(EMovementMode NewMovementMode) const;

	UFUNCTION(BlueprintCallable, Category = "Movement")
	float GetMovementDirection() const;

	UInventoryComponent* GetInventoryComponent() const { return InventoryComponent; }

	UCombatComponent* GetCombatComponent() const { return CombatComponent; }

	UGrabComponent* GetGrabComponent() const { return GrabComponent; }

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
	UCombatComponent* CombatComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UGrabComponent* GrabComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UInventoryComponent* InventoryComponent;

private:
	void MoveForward(const float Amount);
	void MoveRight(const float Amount);
	FRotator GetYawBasedRotator() const;

	void ChangeRunState() { bIsRun = !bIsRun; }

	UFUNCTION()
	void OnDeath();

private:
	bool bIsRun = false;
};
