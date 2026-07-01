#pragma once

#include "CoreMinimal.h"
#include "BuffMage/DataAssets/WeaponDataAsset.h"
#include "RangedWeaponData.generated.h"

UCLASS()
class BUFFMAGE_API URangedWeaponData : public UWeaponDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ProjectileVelocity = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Attack", meta=(MustImplement=Projectile))
	TSubclassOf<AActor> Projectile;

	virtual void Attack(float AddedDamage,FVector StartPos, FRotator Rotation, AActor* Instigator, TArray<AActor*>& ActorsHit) override;
};
