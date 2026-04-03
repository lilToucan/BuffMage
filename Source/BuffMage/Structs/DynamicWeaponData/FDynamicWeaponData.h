#pragma once
#include "BuffMage/DataAssets/WeaponDataAsset.h"

#include "FDynamicWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FDynamicWeaponData
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AttackComponent")
	UWeaponDataAsset* WeaponData = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int AnimIndex = 0;
	
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int IdleAnimIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int WalkAnimIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float CooldownTime = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float CurrentAmmo;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	bool bIsReloading = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	bool bIsAttacking = false;

};
