// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffMage/DataAssets/WeaponDataAsset.h"
#include "Components/ActorComponent.h"
#include "AttackComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), BlueprintType)
class BUFFMAGE_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Weapon")
	TObjectPtr<UWeaponDataAsset> WeaponData;

	UPROPERTY(BlueprintReadWrite, Category="Animations")
	TObjectPtr<UAnimInstance> AnimInstance;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bHasAttackBeenPerformed;

public:
	UAttackComponent();
	void OnMontageNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	void Attack();

protected:
	virtual void BeginPlay() override;
};
