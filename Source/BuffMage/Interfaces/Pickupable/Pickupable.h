#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Character/BuffMageCharacter.h"
#include "UObject/Interface.h"
#include "Pickupable.generated.h"
UINTERFACE()
class UPickupable : public UInterface
{
	GENERATED_BODY()
};

class BUFFMAGE_API IPickupable
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void PickUp(ABuffMageCharacter* Player);
};
