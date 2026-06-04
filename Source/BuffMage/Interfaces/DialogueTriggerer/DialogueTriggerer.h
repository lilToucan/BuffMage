#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "DialogueTriggerer.generated.h"

UINTERFACE(blueprintable, BlueprintType)
class UDialogueTriggerer : public UInterface
{
	GENERATED_BODY()
};

class BUFFMAGE_API IDialogueTriggerer
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Dialogue")
	void TriggerDialogue();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Dialogue")
	void DialogueFailed();
};
