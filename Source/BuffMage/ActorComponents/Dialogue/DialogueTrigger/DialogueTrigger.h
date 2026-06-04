#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/DialogueTriggerer/DialogueTriggerer.h"
#include "Components/ActorComponent.h"
#include "DialogueTrigger.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UDialogueTrigger : public UActorComponent, public IDialogueTriggerer
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (RowType ="FPhrase"))
	TArray<UDataTable*> Dialogues;

	UPROPERTY(BlueprintReadWrite, Category = "Dialogue")
	int32 DialogueIndex;

public:
	UDialogueTrigger();
	
	virtual void TriggerDialogue_Implementation();
	virtual void DialogueFailed_Implementation();

};
