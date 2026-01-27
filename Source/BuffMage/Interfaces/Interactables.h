#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Character/BuffMageCharacter.h"
#include "UObject/Interface.h"
#include "Interactables.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI, BlueprintType)
class UInteractables : public UInterface
{
	GENERATED_BODY()
};

class BUFFMAGE_API IInteractables
{
	GENERATED_BODY()

public:
UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact(ABuffMageCharacter* Player);
};
