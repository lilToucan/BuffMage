#pragma once
#include "BuffMage/DataAssets/WeaponDataAsset.h"

#include "FDynamicWeaponData.generated.h"

USTRUCT(BlueprintType)
struct FDynamicWeaponData
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditAnywhere, Category = "AttackComponent")
	UWeaponDataAsset* WeaponData = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int AnimIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float ShootTime = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float CurrentAmmo;
	
};
