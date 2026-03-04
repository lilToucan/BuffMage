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
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons")
	FDynamicWeaponData RageWeapon;
	
	UPROPERTY(BlueprintReadWrite, Category="AttackComponent|Animations")
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(BlueprintReadWrite, Category="AttackComponent|Animations")
	FDynamicWeaponData CurrentWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float RageMeter;
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons")
	float RageAfterGettingHit = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons")
	float RageAfterHitting = 5.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons")
	int RageAmountAfterKilling = 10.f;
	
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int WeaponIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	UCameraComponent* Cam;
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	TArray<AActor*> HitActors;


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
	void SetUpWeapon(FDynamicWeaponData& WeaponAsset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddWeapon(FDynamicWeaponData& NewWeapon);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackCompleted();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AttackCanBeUsedAgain();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartReloading();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ReduceAmmo();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool HasAmmoBeenDepleted();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnDamageReceived(AActor* Actor, float X, const UDamageType* Damage, AController* Controller, AActor* Actor1);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddRage(int Amount);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddToRageAfterKill();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartRage();
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopRage();
	
protected:
	virtual void BeginPlay() override;
	
};
