#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interactables/TimedPickup/TimedPickup.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

UCLASS()
class BUFFMAGE_API AHealthPack : public ATimedPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float HealingAmount;

protected:

public:
	AHealthPack();
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;
protected:
	virtual void BeginPlay() override;
};
