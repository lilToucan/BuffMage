#include "RandomDialogueTrigger.h"

#include "BuffMage/Subsystems/DialogueSystem/DialogueSystem.h"

URandomDialogueTrigger::URandomDialogueTrigger()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URandomDialogueTrigger::TriggerRandomDialogue(ERandomDialogueTriggerType DialogueId)
{
	CurrentData = Dialogues.Find(DialogueId);

	if (!CurrentData)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Uobject Data is invalid!"));
		return;
	}

	TriggerDialogue_Implementation();
}

void URandomDialogueTrigger::DialogueFailed_Implementation()
{
	CurrentData->GetDefaultObject()->CurrentPercentage = CurrentData->GetDefaultObject()->LastPercentage;
}

void URandomDialogueTrigger::TriggerDialogue_Implementation()
{
	float RandomChance = FMath::RandRange(0.f, 1.f);

	if (RandomChance <= CurrentData->GetDefaultObject()->CurrentPercentage)
	{
		const int RandData = FMath::RandRange(0, CurrentData->GetDefaultObject()->Dialogues.Num() - 1);
		UDataTable* DataTable = CurrentData->GetDefaultObject()->Dialogues[RandData];
		GetOwner()->GetWorld()->GetSubsystem<UDialogueSystem>()->StartDialogue(DataTable, this);
		CurrentData->GetDefaultObject()->LastPercentage = CurrentData->GetDefaultObject()->CurrentPercentage;
		CurrentData->GetDefaultObject()->CurrentPercentage = 0;
	}
	else
	{
		CurrentData->GetDefaultObject()->CurrentPercentage += CurrentData->GetDefaultObject()->PercentageAdded;

		if (CurrentData->GetDefaultObject()->CurrentPercentage > CurrentData->GetDefaultObject()->MaxPercentage)
		{
			CurrentData->GetDefaultObject()->CurrentPercentage = CurrentData->GetDefaultObject()->MaxPercentage;
		}
		
		CurrentData->GetDefaultObject()->LastPercentage = CurrentData->GetDefaultObject()->CurrentPercentage;
	}
}
