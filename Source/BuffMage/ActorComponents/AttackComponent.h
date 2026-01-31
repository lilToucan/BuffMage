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
	TArray<TObjectPtr<UWeaponDataAsset>>WeaponData;

	UPROPERTY(BlueprintReadWrite, Category="AttackComponent|Animations")
	TObjectPtr<UAnimInstance> AnimInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AttackComponent|OverlapSphere")
	TArray<TEnumAsByte<ECollisionChannel>> CollisionChannelsToHit;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int AnimIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	int WeaponIndex = 0;

	UPROPERTY(BlueprintReadWrite, Category = "AttackComponent")
	float ShootTime = 0;
	
// functions
public:
	UAttackComponent();
	
	UFUNCTION(BlueprintCallable)
	virtual void StartAttackAnim();

	UFUNCTION(BlueprintCallable)
	virtual void HitDetection(FName SocketName);

	UFUNCTION(BlueprintCallable)
	virtual void ChangeWeapon(int InputValue);

	UFUNCTION(BlueprintCallable)
	virtual void AddWeapon(UWeaponDataAsset* Weapon);
	
protected:
	virtual void BeginPlay() override;
};
