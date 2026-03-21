#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Structs/DynamicWeaponData/FDynamicWeaponData.h"
#include "Components/ActorComponent.h"
#include "Engine/StreamableManager.h"
#include "AttackComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnRageChangeDelegate, float, CurrentAmount, float, MaximumAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChangeDelegate, float, CurrentAmount);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnWeaponIconChanged, TSoftObjectPtr<UTexture2D>, Icon);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnIdleChange, UAnimSequenceBase*, IdleAnim);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAttackHitDelegate);

class UCameraComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType, Blueprintable)
class BUFFMAGE_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

	// VARIABLES:
public:
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable)
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

	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, Category = "AttackComponent|Weapons|Delegate")
	FOnValueChangeDelegate OnAmmoChange;
	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, Category = "AttackComponent|Weapons|Delegate")
	FOnWeaponIconChanged OnWeaponIconChanged;

	UPROPERTY(BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, Category = "AttackComponent|Weapons|Delegate")
	FOnIdleChange OnIdleAnimChanged;

#pragma endregion
	//TODO: i feel like the rage things should be it's onw components
#pragma region RAGE
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage")
	FDynamicWeaponData RageWeapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackComponent|Weapons|Rage|Debug")
	float CurrentRageAmount = 0.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackComponent|Weapons|Rage|Debug")
	float CurrentRageTime = 0.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AttackComponent|Weapons|Rage|Debug")
	bool bIsInRage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageDuration = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageMaxPoints = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageAmountAfterKilling = 10.f;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageAfterGettingHit = 2.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	float RageAfterHitting = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Config")
	bool bRemoveRageWhenStopped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Sound")
	float RageVolume = .15f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Sound")
	float RageMaxPitch = 1.7f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Sound")
	float RageMinPitch = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapons|Rage|Sound")
	TSoftObjectPtr<USoundBase> StartRageSound;
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
#pragma region DefaultFunctions

public:
	UAttackComponent();

protected:
	virtual void BeginPlay() override;
#pragma endregion

public:
#pragma region SoundFunctions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PlayAudio();

	UFUNCTION(BlueprintCallable)
	void LoadSoundAsync();
#pragma endregion

#pragma region AttackFunctions
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartAttackAnim();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void HitDetection(FName SocketName);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ResetHitActors();
#pragma endregion

	UFUNCTION(BlueprintCallable)
	virtual void SetUpWeapon(FDynamicWeaponData& WeaponAsset);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ChangeWeapon(int InputValue);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetCooldownTime();

	UFUNCTION(BlueprintCallable)
	virtual void AddWeapon(FDynamicWeaponData& NewWeapon);

	UFUNCTION(BlueprintCallable)
	void UpdateIdle();

	UFUNCTION(BlueprintCallable)
	void OnCurrentWeaponChange();

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
	void OnAttackHit(AActor* ActorHit);

#pragma region RageFunctions

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void SetRage(float RageValue);
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void RemoveRageTime(float DeltaTime);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddRage(float Amount);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void AddToRageAfterKill();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StartRage();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void StopRage();

protected:
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
#pragma endregion
};
