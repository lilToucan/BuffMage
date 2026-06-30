#pragma once
#include "CoreMinimal.h"
#include "BuffMage/Interactables/TimedPickup/TimedPickup.h"
#include "BuffMage/Interfaces/Pickupable/Pickupable.h"
#include "GameFramework/Actor.h"
#include "ShieldPack.generated.h"

UCLASS()
class BUFFMAGE_API AShieldPack : public ATimedPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float ShieldAmount;

public:
	AShieldPack();
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;
protected:
	virtual void BeginPlay() override;
};
