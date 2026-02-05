#pragma once

#include "CoreMinimal.h"
#include "BuffMage/DataAssets/Weapons/MeleeWeaponData.h"
#include "StunWeapon.generated.h"

UCLASS()
class BUFFMAGE_API UStunWeapon : public UMeleeWeaponData
{
	GENERATED_BODY()

	virtual void Attack(FVector StartPos, FRotator Rotation, AActor* Instigator) override;
};
