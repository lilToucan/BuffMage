#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/Pickupable/Pickupable.h"
#include "GameFramework/Actor.h"
#include "Booster.generated.h"

UCLASS()
class BUFFMAGE_API ABooster : public AActor, public IPickupable
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, blueprintreadwrite)
	float BoostAmount;
	UPROPERTY(EditAnywhere, blueprintreadwrite)
	float Duration;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float OriginalBoostedValue;

	UPROPERTY()
	FTimerHandle Timer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ABuffMageCharacter* PlayerCharacter;

public:
	ABooster();
	UFUNCTION(BlueprintCallable)
	virtual void ResetBoost();
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void DisableActor();
	virtual void BeginPlay() override;
};
