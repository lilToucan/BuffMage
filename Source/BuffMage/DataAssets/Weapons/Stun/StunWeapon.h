#pragma once

#include "CoreMinimal.h"
#include "BuffMage/DataAssets/Weapons/MeleeWeaponData.h"
#include "StunWeapon.generated.h"

UCLASS()
class BUFFMAGE_API UStunWeapon : public UMeleeWeaponData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float StunDuration;
	
	virtual void Attack(FVector StartPos, FRotator Rotation, AActor* Instigator,TArray<AActor*>& ActorsHit) override;
};
