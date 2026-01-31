#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"


UCLASS(BlueprintType)
class BUFFMAGE_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float PositionOffsetX = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float FireRate = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float AmmoMax = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")  // could be animation notify
	float ReloadTime = 10;

	// only ranged
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float ProjectileVelocity = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackComboAnimMontage;

	virtual void Attack(FVector StartPos, TArray<TEnumAsByte<ECollisionChannel>>& CollisionChannel, AActor* Instigator);
};
