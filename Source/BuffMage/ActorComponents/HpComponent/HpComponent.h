#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HpComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStunnedDelegate);

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

	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadWrite, Category = "HpComponent|Stun")
	FOnStunnedDelegate OnStunned;
	UPROPERTY(BlueprintCallable,BlueprintAssignable,BlueprintReadWrite, Category = "HpComponent|Stun")
	FOnStunnedDelegate OnStunRecovered;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HpComponent|Hp")
	float CurrentHp;

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Animations")
	ACharacter* CharacterOwner;
	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Animations")
	USkeletalMesh* OwnersSkeletalMesh;

	UPROPERTY(BlueprintReadOnly, Category = "HpComponent|Animations")
	float DeathAnimDuration;
	
	FTimerHandle StunTimerHandle;
	
	bool bIsActive;

	// functions
public:
	UHpComponent();

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
	
	UFUNCTION(BlueprintCallable, Category = "HpComponent")
	void OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "HpComponent")
	void OnHealingTaken(float Healing, AActor* HealingCauser);

	void RecoverFromStun();
	UFUNCTION(BlueprintCallable, Category = "HpComponent|Stun")
	virtual void GetStunned(float Time, AActor* Instigator);

protected:
	virtual void BeginPlay() override;
	void DisableOwner();
	void OnAnimNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void Death();
	void StartDeathTimer();
};
