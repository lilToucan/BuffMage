// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffMage/DataAssets/WeaponDataAsset.h"
#include "CapsuleWeaponData.generated.h"

UCLASS()
class BUFFMAGE_API UCapsuleWeaponData : public UWeaponDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Radius = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<TEnumAsByte<EObjectTypeQuery>> CollisionChannel;

	virtual void Attack(float AddedDamage, FVector StartPos, FRotator Rotation, AActor* Instigator, TArray<AActor*>& ActorsHit) override;
};
