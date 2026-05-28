#include "InteractableDialogueTrigger.h"

#include "BuffMage/Subsystems/DialogueSystem/DialogueSystem.h"


UInteractableDialogueTrigger::UInteractableDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableDialogueTrigger::TriggerDialogue_Implementation()
{
	GetOwner()->GetWorld()->GetSubsystem<UDialogueSystem>()->StartDialogue(Dialogues[DialogueIndex]);

	DialogueIndex++;

	if (DialogueIndex >= Dialogues.Num())
		DialogueIndex = 0;
}

void UInteractableDialogueTrigger::Interact_Implementation(ABuffMageCharacter* Player)
{
	TriggerDialogue_Implementation();
}
