#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Structs/DynamicWeaponData/FDynamicWeaponData.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class BUFFMAGE_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()
// variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons")
	bool FireRateActive;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons")
	TArray<FDynamicWeaponData>WeaponsData;

	UPROPERTY(BlueprintReadWrite, Category="AttackComponent|Animations")
	TObjectPtr<UAnimInstance> AnimInstance;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int WeaponIndex = 0;
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	UCameraComponent* Cam;
	
	
// functions
public:
	UAttackComponent();
	
	UFUNCTION(BlueprintCallable)
	virtual void StartAttackAnim();

	UFUNCTION(BlueprintCallable)
	virtual void HitDetection(FName SocketName);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeWeapon(int InputValue);
	void SetUpWeapon(FDynamicWeaponData& Weapon);

	UFUNCTION(BlueprintCallable)
	virtual void AddWeapon(FDynamicWeaponData& Weapon);
	
	UFUNCTION(BlueprintCallable)
	virtual void ReloadWeapon();
	
	UFUNCTION(BlueprintCallable)
	virtual void AttackCompleted();
	
protected:
	virtual void BeginPlay() override;
};
