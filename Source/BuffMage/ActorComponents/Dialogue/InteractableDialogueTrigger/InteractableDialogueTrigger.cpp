#include "InteractableDialogueTrigger.h"

#include "BuffMage/Subsystems/DialogueSystem/DialogueSystem.h"


UInteractableDialogueTrigger::UInteractableDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UInteractableDialogueTrigger::TriggerDialogue_Implementation()
{
	GetOwner()->GetWorld()->GetSubsystem<UDialogueSystem>()->StartDialogue(Dialogues[DialogueIndex],this);

	DialogueIndex++;

	if (DialogueIndex >= Dialogues.Num())
		DialogueIndex = 0;
}

void UInteractableDialogueTrigger::DialogueFailed_Implementation()
{
	Super::DialogueFailed_Implementation();
}

void UInteractableDialogueTrigger::Interact_Implementation(ABuffMageCharacter* Player)
{
	TriggerDialogue_Implementation();
}
