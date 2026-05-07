#include "DialogueTrigger.h"


UDialogueTrigger::UDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UDialogueTrigger::BeginPlay()
{
	Super::BeginPlay();
}

void UDialogueTrigger::TriggerDialogue_Implementation()
{
	//GetOwner()->GetWorld()->GetSubsystem<UDialogueSystem>()->StartDialogue();
}

