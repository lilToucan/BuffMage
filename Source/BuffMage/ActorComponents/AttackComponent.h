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
// variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|Weapon")
	TObjectPtr<UWeaponDataAsset> WeaponData;

	UPROPERTY(BlueprintReadWrite, Category="AttackComponent|Animations")
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="AttackComponent|OverlapSphere")
	float OverlapSphereRange = 500;
	UPROPERTY(EditAnywhere,BlueprintReadWrite, Category="AttackComponent|OverlapSphere")
	TEnumAsByte<ECollisionChannel> CollisionChannelToHit;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "Attack")
	bool bHasAttackBeenPerformed;

// functions
public:
	UAttackComponent();
	virtual void OnMontageNotifyBegin(FName Name,float BlendTime, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	virtual void Attack();
	virtual void DealDamage();
	virtual void CheckComboPerformed(float BlendTime);

protected:
	virtual TArray<AActor*> AttackOverlapSphere();
	virtual void BeginPlay() override;
};
