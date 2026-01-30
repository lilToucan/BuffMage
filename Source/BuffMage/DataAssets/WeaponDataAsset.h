#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "WeaponDataAsset.generated.h"


UCLASS()
class BUFFMAGE_API UWeaponDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Damage = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Range = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float PositionOffsetX = 400;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float FireRate = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack", meta= (ToolTip="the angle you want the player to check after performing an attack"))
	float AngleDetection = 180;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	TArray<TObjectPtr<UAnimMontage>> AttackComboAnimMontage;

	void Attack(FVector StartPos, TArray<TEnumAsByte<EObjectTypeQuery>>& CollisionChannel);
};
