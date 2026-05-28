#include "DialogueTrigger.h"

#include "BuffMage/Subsystems/DialogueSystem/DialogueSystem.h"


UDialogueTrigger::UDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueTrigger::TriggerDialogue_Implementation()
{

	GetOwner()->GetWorld()->GetSubsystem<UDialogueSystem>()->StartDialogue(Dialogues[DialogueIndex]);

	DialogueIndex++;

	if (DialogueIndex >= Dialogues.Num())
		DialogueIndex = 0;
}
