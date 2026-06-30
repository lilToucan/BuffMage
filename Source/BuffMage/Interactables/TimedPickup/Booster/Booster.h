#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interactables/TimedPickup/TimedPickup.h"
#include "BuffMage/Interfaces/Pickupable/Pickupable.h"
#include "GameFramework/Actor.h"
#include "Booster.generated.h"

UCLASS()
class BUFFMAGE_API ABooster : public ATimedPickup
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, blueprintreadwrite,Category ="Config")
	float BoostAmount;
	UPROPERTY(EditAnywhere, blueprintreadwrite,Category ="Config")
	float Duration;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite,Category ="Config")
	float OriginalBoostedValue;

	UPROPERTY()
	FTimerHandle Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category ="Config")
	ABuffMageCharacter* PlayerCharacter;

public:
	ABooster();
	UFUNCTION(BlueprintCallable)
	virtual void ResetBoost();
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;

protected:
	virtual void BeginPlay() override;
};
