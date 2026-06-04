#include "DialogueTrigger.h"

#include "BuffMage/Subsystems/DialogueSystem/DialogueSystem.h"


UDialogueTrigger::UDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UDialogueTrigger::DialogueFailed_Implementation()
{
	DialogueIndex--;

	if (DialogueIndex < 0)
		DialogueIndex = Dialogues.Num() - 1;
}

void UDialogueTrigger::TriggerDialogue_Implementation()
{
	GetOwner()->GetWorld()->GetSubsystem<UDialogueSystem>()->StartDialogue(Dialogues[DialogueIndex],this);

	DialogueIndex++;

	if (DialogueIndex >= Dialogues.Num())
		DialogueIndex = 0;
}
