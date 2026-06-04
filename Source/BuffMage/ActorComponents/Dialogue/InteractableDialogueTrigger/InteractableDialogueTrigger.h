#pragma once

#include "CoreMinimal.h"
#include "BuffMage/ActorComponents/Dialogue/DialogueTrigger/DialogueTrigger.h"
#include "BuffMage/Interfaces/Interactables.h"
#include "InteractableDialogueTrigger.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UInteractableDialogueTrigger : public UDialogueTrigger, public IInteractables
{
	GENERATED_BODY()

public:
	UInteractableDialogueTrigger();
	
	virtual void TriggerDialogue_Implementation() override;
	virtual void DialogueFailed_Implementation() override;

	virtual void Interact_Implementation(ABuffMageCharacter* Player) override;
	
};
