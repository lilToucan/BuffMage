#pragma once

#include "CoreMinimal.h"
#include "BuffMage/DataAssets/WeaponDataAsset.h"
#include "MeleeWeaponData.generated.h"

UCLASS()
class BUFFMAGE_API UMeleeWeaponData : public UWeaponDataAsset
{
	GENERATED_BODY()

	public:
	// only melee or something like a bazooka (R.I.P Granny)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range = 400;

	// only melee or something like a bazooka (R.I.P Granny)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta= (ToolTip="the angle you want the player to check after performing an attack", ClampMax = 360.f, ClampMin = 0.f, ClampMax = 360.f))
	float AngleDetection = 180;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	/*TArray<TEnumAsByte<ECollisionChannel>>*/TArray<TEnumAsByte<EObjectTypeQuery>> CollisionChannel;

	
	virtual void Attack(float AddedDamage,FVector StartPos, FRotator Rotation, AActor* Instigator, TArray<AActor*>& ActorsHit) override;
};
