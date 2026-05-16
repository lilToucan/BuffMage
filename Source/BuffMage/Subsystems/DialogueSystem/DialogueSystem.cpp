#include "DialogueSystem.h"


void UDialogueSystem::StartDialogue(UDataTable* Dialogue)
{
	FDialogue DialogueStruct;
	TArray<FPhrase*> PhraseArray;
	Dialogue->GetAllRows<FPhrase>("Getting Phreses out of dialogue", PhraseArray);

	for (FPhrase* line : PhraseArray)
	{
		DialogueStruct.Conversation.Add(*line);
	}

	OnStartDialogue.Broadcast(DialogueStruct);
	
}
