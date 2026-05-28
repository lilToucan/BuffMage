#include "RandomDialogueTrigger.h"

#include "BuffMage/Subsystems/DialogueSystem/DialogueSystem.h"

URandomDialogueTrigger::URandomDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URandomDialogueTrigger::TriggerDialogue(ERandomDialogueTriggerType DialogueId)
{
	URandomDialogueData* Data = *Dialogues.Find(DialogueId);
	
	if (!Data)
	{
		if(GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Uobject Data is invalid!"));	
		return;
	}
	
	float Rand = FMath::RandRange(0.f, 1.f);
	
	if (Rand <= Data->CurrentPercentage)
	{
		GetOwner()->GetWorld()->GetSubsystem<UDialogueSystem>()->StartDialogue(Data->Dialogues[FMath::RandRange(0, Data->Dialogues.Num() - 1)]);
		Data->CurrentPercentage = 0;
	}
	else
	{
		Data->CurrentPercentage += Data->PercentageAdded;
		
		if (Data->CurrentPercentage > Data->MaxPercentage)
		{
			Data->CurrentPercentage = Data->MaxPercentage;
		}
	}
	
}
