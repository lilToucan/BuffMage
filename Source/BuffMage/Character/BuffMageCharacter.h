#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "BuffMage/Interfaces/AttackerInterface.h"
#include "GameFramework/Character.h"
#include "BuffMage/ActorComponents/AttackComponent.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component|AttackComponent")
	TObjectPtr<UAttackComponent> AttackComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="DashComponent")
	TObjectPtr<UDashComponent> DashComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Component|AttackComponent")
	TObjectPtr<UHpComponent> HpComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Input")
	UInputAction* MoveInputAction;

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

public: // functions
	ABuffMageCharacter();
	virtual void Tick(float DeltaTime) override;
	virtual void InteractInputFunction(const FInputActionValue& InputActionValue);
	virtual void ChangeWeaponInputFunction(const FInputActionValue& InputActionValue);
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void MoveInputFunction(const FInputActionValue& InputActionValue);
	virtual void AimInputFunction(const FInputActionValue& InputActionValue);
	virtual void AttackInputFunction(const FInputActionValue& InputActionValue);

protected: // functions
	virtual void BeginPlay() override;
};
