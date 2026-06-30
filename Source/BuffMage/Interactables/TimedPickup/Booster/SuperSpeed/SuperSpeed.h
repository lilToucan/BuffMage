#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interactables/TimedPickup/Booster/Booster.h"
#include "SuperSpeed.generated.h"

UCLASS()
class BUFFMAGE_API ASuperSpeed : public ABooster
{
	GENERATED_BODY()

public:
	ASuperSpeed();
	virtual void ResetBoost() override;
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;

protected:
	virtual void BeginPlay() override;
};
