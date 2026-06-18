#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHPDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHPValueCahngeDelegate, float, CurrentHp);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHitDelegate, AActor*, HitActor, bool, bAppliesRage);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UHpComponent : public UActorComponent
{
	GENERATED_BODY()

	// variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Config")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Config")
	float StartingHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Hit")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Death")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Death")
	FName DeathNotifyName = FName("DeathNotify");

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Death")
	FTimerHandle TimerHandle;

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Stun")
	FTimerHandle StunTimerHandle;
	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Stun")
	bool bIsStunned = false;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, BlueprintReadWrite, Category = "HpComponent|Delegate")
	FHPDelegate OnStunned;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, BlueprintReadWrite, Category = "HpComponent|Delegate")
	FHPDelegate OnStunRecovered;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, BlueprintReadWrite, Category = "HpComponent|Delegate")
	FHPDelegate OnDeathStarted;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, BlueprintReadWrite, Category = "HpComponent|Delegate")
	FHPDelegate OnDeathEnd;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, BlueprintReadWrite, Category = "HpComponent|Delegate")
	FHPDelegate OnHit;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, BlueprintReadWrite, Category = "HpComponent|Delegate")
	FHPValueCahngeDelegate OnHpChanged;
	UPROPERTY(BlueprintCallable, BlueprintAssignable, BlueprintReadWrite, Category = "HpComponent|Delegate")
	FHPValueCahngeDelegate OnHpDirectlyChanged;

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Rage")
	bool bAppliesRage = true;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Sound|Hurt")
	TArray<USoundBase*> HurtSounds;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Sound|Hurt")
	FVector2D HurtVolumeMinMax = FVector2D(1.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Sound|Hurt")
	FVector2D HurtPitchMinMax = FVector2D(0.7f, 1.3f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Sound|Death")
	USoundBase* DeathSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Sound|Death")
	FVector2D DeathVolumeMinMax = FVector2D(1.f, 1.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Sound|Death")
	FVector2D DeathPitchMinMax = FVector2D(0.7f, 1.3f);

	

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HpComponent|Hp")
	float CurrentHp;

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Animations")
	ACharacter* CharacterOwner;
	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Animations")
	USkeletalMesh* OwnersSkeletalMesh;

	UPROPERTY(BlueprintReadOnly, Category = "HpComponent|Animations")
	float DeathAnimDuration;

	// functions
public:
	UHpComponent();

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

	UFUNCTION(BlueprintCallable, Category = "HpComponent")
	virtual void OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "HpComponent")
	virtual void ApplyDamageDirectly(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser, bool bShouldFade);

	UFUNCTION(BlueprintCallable, Category = "HpComponent")
	virtual void OnHealingTaken(float Healing, AActor* HealingCauser);

	UFUNCTION(BlueprintCallable, Category = "HpComponent|Death")
	virtual void Death(AActor* TheKiller);

	UFUNCTION()
	virtual void RecoverFromStun();

	UFUNCTION(BlueprintCallable, Category = "HpComponent|Stun")
	virtual void GetStunned(float Time, AActor* Instigator);

protected:
	UFUNCTION()
	// void OnDeathNotify(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);

	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = "HpComponent|Sound")
	virtual void PlaySound(USoundBase* CurrentSoundToPlay, AActor* HitActor, float Volume, float Pitch, USoundAttenuation* SoundAttenuation = nullptr);
	virtual void DisableOwner();
	virtual void StartDeathTimer();
};
