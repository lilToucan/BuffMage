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
	TArray<FDynamicWeaponData> WeaponsData;

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

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAttackAnim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HitDetection(FName SocketName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ChangeWeapon(int InputValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetUpWeapon(FDynamicWeaponData& Weapon);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddWeapon(FDynamicWeaponData& Weapon);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackCompleted();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartReloading(UWeaponDataAsset* Weapon);
	
protected:
	virtual void BeginPlay() override;
};
