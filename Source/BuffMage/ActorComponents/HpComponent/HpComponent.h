// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HpComponent.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Config")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Config")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HpComponent|Config")
	FName DeathNotifyName = FName("DeathNotify");

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Config")
	FTimerHandle TimerHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "HpComponent|Hp")
	float CurrentHp;

	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Animations")
	USkeletalMesh* OwnersSkeletalMesh;
	UPROPERTY(BlueprintReadWrite, Category = "HpComponent|Animations")
	ACharacter* CharacterOwner;
	UPROPERTY(BlueprintReadOnly, Category = "HpComponent|Animations")
	float DeathAnimDuration;

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

protected:
	virtual void BeginPlay() override;
	void DisableOwner();
	void OnAnimNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void Death();
	void StartDeathTimer();
};
