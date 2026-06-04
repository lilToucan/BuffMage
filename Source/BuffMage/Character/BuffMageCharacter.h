#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "GameFramework/Character.h"
#include "BuffMage/ActorComponents/Attack/AttackComponent.h"
#include "BuffMage/ActorComponents/HpComponent/HpComponent.h"
#include "BuffMage/ActorComponents/Dash/DashComponent.h"

#include "BuffMageCharacter.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVoidFloat, float, Velocity);


UCLASS(abstract)
class BUFFMAGE_API ABuffMageCharacter : public ACharacter
{
	GENERATED_BODY()

public: // variables
	
	UPROPERTY(BlueprintAssignable, Category = "Input")
	FVoidFloat OnMovementInput;

protected: // variables
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Interact")
	TEnumAsByte<ECollisionChannel> InteractCollisionChannel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Interact")
	float InteractionRadius = 750.f;

	UPROPERTY(BlueprintreadWrite, Category="Movement")
	float Velocity;
	
	
	// components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|AttackComponent")
	TObjectPtr<UAttackComponent> AttackComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|DashComponent")
	TObjectPtr<UDashComponent> DashComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|HpComponent")
	TObjectPtr<UHpComponent> HpComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components|CameraComponent")
	TObjectPtr<UCameraComponent> CameraComponent;

	// inputs
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Basics")
	UInputAction* MoveInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Basics")
	UInputAction* JumpInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Aim")
	UInputAction* LookAroundInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Shoot")
	UInputAction* ShootInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Dash")
	UInputAction* DashInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Interact")
	UInputAction* InteractInputAction;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|ChangeWeapon")
	UInputAction* ChangeWeaponInputAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Reload")
	UInputAction* ReloadInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input|Rage")
	UInputAction* RageInputAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* PauseInput;

public: // functions
	ABuffMageCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void InteractInputFunction(const FInputActionValue& InputActionValue);
	virtual void ChangeWeaponInputFunction(const FInputActionValue& InputActionValue);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MoveInputFunction(const FInputActionValue& InputActionValue);
	virtual void AimInputFunction(const FInputActionValue& InputActionValue);
	virtual void AttackInputFunction(const FInputActionValue& InputActionValue);
	virtual void ReloadInputFunction(const FInputActionValue& InputActionValue);
	virtual void RageInputFunction(const FInputActionValue& InputActionValue);
	virtual void JumpInputFunction(const FInputActionValue& InputActionValue);

	UFUNCTION(BlueprintNativeEvent)
	void PauseFunction(const FInputActionValue& InputActionValue);
	

protected: // functions

	virtual void BeginPlay() override;
	UFUNCTION()
	void OnStunned();
	UFUNCTION()
	void OnStunRecovered();
};
