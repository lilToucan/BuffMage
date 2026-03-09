#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOHPDelegate);

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
	
	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadWrite, Category = "HpComponent|Stun")
	FOHPDelegate OnStunned;
	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadWrite, Category = "HpComponent|Stun")
	FOHPDelegate OnStunRecovered;

	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadWrite, Category = "HpComponent|Stun")
	FOHPDelegate OnDeathStarted;
	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadWrite, Category = "HpComponent|Stun")
	FOHPDelegate OnDeathEnd;
	
	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadWrite, Category = "HpComponent|Stun")
	FOHPDelegate OnStartHitAnim;

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Rage")
	bool bAppliesRage = true;
	
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
	void OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "HpComponent")
	void OnHealingTaken(float Healing, AActor* HealingCauser);

	UFUNCTION(BlueprintCallable, Category = "HpComponent|Death")
	void Death(AActor* TheKiller);

	UFUNCTION()
	void RecoverFromStun();
	
	UFUNCTION(BlueprintCallable, Category = "HpComponent|Stun")
	virtual void GetStunned(float Time, AActor* Instigator);


protected:
	UFUNCTION()
	void OnDeathNotify(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	
	virtual void BeginPlay() override;
	void DisableOwner();
	void StartDeathTimer();
};
