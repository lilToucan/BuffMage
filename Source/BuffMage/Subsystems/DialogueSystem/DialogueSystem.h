#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Structs/Dialogue/FDialogue.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.generated.h"

class IDialogueTriggerer;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnNextPhraseDelegate, FDialogue, Dialogue,TScriptInterface<IDialogueTriggerer>,Trigger);

UCLASS()
class BUFFMAGE_API UDialogueSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "DialogueSystem")
	FOnNextPhraseDelegate OnStartDialogue;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem")
	void StartDialogue(
		/*pass an array of phrases = string text , float time*/
		UPARAM(meta = (RowType ="FPhrase")) UDataTable* Dialogue, TScriptInterface<IDialogueTriggerer> DialogueTrigger);
};
