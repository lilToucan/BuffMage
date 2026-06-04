#include "DialogueSystem.h"

#include "BuffMage/Interfaces/DialogueTriggerer/DialogueTriggerer.h"


void UDialogueSystem::StartDialogue(UDataTable* Dialogue, TScriptInterface<IDialogueTriggerer> DialogueTrigger)
{
	FDialogue DialogueStruct;
	TArray<FPhrase*> PhraseArray;
	Dialogue->GetAllRows<FPhrase>("Getting Phreses out of dialogue", PhraseArray);

	for (FPhrase* line : PhraseArray)
	{
		DialogueStruct.Conversation.Add(*line);
	}

	OnStartDialogue.Broadcast(DialogueStruct,DialogueTrigger);
	
}
