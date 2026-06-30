#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interactables/TimedPickup/Booster/Booster.h"
#include "GameFramework/Actor.h"
#include "SuperJump.generated.h"

UCLASS()
class BUFFMAGE_API ASuperJump : public ABooster
{
	GENERATED_BODY()

public:
	ASuperJump();
	virtual void ResetBoost() override;
	
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;

protected:
	virtual void BeginPlay() override;
};
