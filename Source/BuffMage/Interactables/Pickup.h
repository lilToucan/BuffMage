#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/Pickupable/Pickupable.h"
#include "GameFramework/Actor.h"
#include "Pickup.generated.h"

UCLASS(Blueprintable, BlueprintType)
class BUFFMAGE_API APickup : public AActor, public IPickupable
{
	GENERATED_BODY()

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;

	UFUNCTION(BlueprintNativeEvent)
	void DisableActor();
};
