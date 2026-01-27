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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|HPComponent|Config")
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|HPComponent|Config")
	float StartingHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|HPComponent|Config")
	UAnimMontage* HitMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|HPComponent|Config")
	UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Component|HPComponent|Config")
	FName DeathNotifyName = FName("DeathNotify");

	UPROPERTY(BlueprintReadWrite, Category = "Component|HPComponent|Config")
	FTimerHandle TimerHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component|HPComponent|Hp")
	float CurrentHp;

	UPROPERTY(BlueprintReadWrite, Category = "Component|HPComponent|Animations")
	USkeletalMesh* OwnersSkeletalMesh;
	UPROPERTY(BlueprintReadWrite, Category = "Component|HPComponent|Animations")
	ACharacter* CharacterOwner;
	UPROPERTY(BlueprintReadOnly, Category = "Component|HPComponent|Animations")
	float DeathAnimDuration;

	bool bIsActive;

	// functions
public:
	UHpComponent();

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;
	
	UFUNCTION(BlueprintCallable, Category = "Component|HPComponent")
	void OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy,
	                   AActor* DamageCauser);

	UFUNCTION(BlueprintCallable, Category = "Component|HPComponent")
	void OnHealingTaken(float Healing, AActor* HealingCauser);

protected:
	virtual void BeginPlay() override;
	void DisableOwner();
	void OnAnimNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void Death();
	void StartDeathTimer();
};
