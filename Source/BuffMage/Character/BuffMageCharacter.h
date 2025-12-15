// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"

#include "BuffMageCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidFloat, float, Velocity);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FVoid);

UCLASS()
class BUFFMAGE_API ABuffMageCharacter : public ACharacter
{
	GENERATED_BODY()

public: // variables
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FVoidFloat OnMovementInput;

	UPROPERTY(BlueprintAssignable, Category = "Input")
	FVoid OnAttackInput;
	
protected: // variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAroundInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ShootInputAction;

	UPROPERTY(BlueprintreadWrite, Category="Input")
	float Velocity;

public: // functions
	ABuffMageCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void MoveInputFunction(const FInputActionValue& InputActionValue);
	void AimInputFunction(const FInputActionValue& InputActionValue);
	void AttackInputFunction(const FInputActionValue& InputActionValue) const;
protected: // functions
	virtual void BeginPlay() override;
};
