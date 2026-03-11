#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Structs/DynamicWeaponData/FDynamicWeaponData.h"
#include "Components/ActorComponent.h"
#include "Engine/StreamableManager.h"
#include "AttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChangeDelegate, float, CurrentAmount, float, MaximumAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackHitDelegate);

class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class BUFFMAGE_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	// VARIABLES:
public:
	FOnRageChangeDelegate OnRageChange;

	UPROPERTY(BlueprintReadWrite, Category="AttackComponent|Animations")
	TObjectPtr<UAnimInstance> AnimInstance;

protected:
#pragma region WEAPONS
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons")
	TArray<FDynamicWeaponData> WeaponsData;
	UPROPERTY(BlueprintReadWrite, Category="AttackComponent|Weapons")
	FDynamicWeaponData CurrentWeapon;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent|Weapons")
	int WeaponIndex = 0;
#pragma endregion

	//TODO: i feel like the rage things should be it's onw components
#pragma region RAGE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage")
	FDynamicWeaponData RageWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackComponent|Weapons|Rage|Debug")
	float CurrentRageAmount = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackComponent|Weapons|Rage|Debug")
	bool bIsInRage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageDuration = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageAmountAfterKilling = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageAfterGettingHit = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageAfterHitting = 5.0f;
#pragma endregion

	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable)
	FOnAttackHitDelegate OnAttackHitDel;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	UCameraComponent* Cam;
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	TArray<AActor*> HitActors;
	
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	TSoftObjectPtr<USoundBase> CurrentSoundToPlay;
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float Volume;
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float Pitch;
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	AActor* HitActor;

	// FUNCTIONS:
public:
	UAttackComponent();
	
	UFUNCTION(BlueprintCallable)
	void PlayAudio();

	UFUNCTION(BlueprintCallable)
	virtual void SetUpWeapon(FDynamicWeaponData& WeaponAsset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAttackAnim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HitDetection(FName SocketName);

	UFUNCTION(BlueprintCallable,BlueprintNativeEvent)
	void ResetHitActors();
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ChangeWeapon(int InputValue);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCooldownTime();

	UFUNCTION(BlueprintCallable)
	virtual void AddWeapon(FDynamicWeaponData& NewWeapon);

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
	void AddRage(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddToRageAfterKill();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartRage();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopRage();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void OnAttackHit(AActor* ActorHit);

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
