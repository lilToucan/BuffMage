// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "BuffMage/Interfaces/AttackerInterface.h"
#include "GameFramework/Character.h"
#include "BuffMage/ActorComponents/AttackComponent.h"

#include "BuffMageCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidFloat, float, Velocity);

UCLASS(abstract)
class BUFFMAGE_API ABuffMageCharacter : public ACharacter, public IAttackerInterface
{
	GENERATED_BODY()

public: // variables
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FVoidFloat OnMovementInput;

	
	
	
protected: // variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent")
	TObjectPtr<UAttackComponent> AttackComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* LookAroundInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* ShootInputAction;
	
	UPROPERTY(BlueprintreadWrite, Category="Movement")
	float Velocity;


	
public: // functions
	ABuffMageCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MoveInputFunction(const FInputActionValue& InputActionValue);
	virtual void AimInputFunction(const FInputActionValue& InputActionValue);
	virtual void AttackInputFunction(const FInputActionValue& InputActionValue);
	
protected: // functions
	UFUNCTION()
	virtual void OnMontageNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnMonatageNotifyBegin();
	virtual TObjectPtr<UAttackComponent> GetAttackComponent() override;
	virtual void BeginPlay() override;
};
