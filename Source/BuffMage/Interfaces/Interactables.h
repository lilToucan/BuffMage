#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Interactables.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractables : public UInterface
{
	GENERATED_BODY()
};

class BUFFMAGE_API IInteractables
{
	GENERATED_BODY()

public:
UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Interact();
};
