#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interactables/Pickup.h"
#include "TimedPickup.generated.h"

UCLASS()
class BUFFMAGE_API ATimedPickup : public APickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="Config")
	float RespawnTime;

protected:
	FTimerHandle RespawnTimer;

public:
	ATimedPickup();

protected:
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;

	UFUNCTION(BlueprintNativeEvent)
	void Respawn();

	virtual void DisableActor_Implementation() override;
};
