#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/DialogueTriggerer/DialogueTriggerer.h"
#include "Components/ActorComponent.h"
#include "TEST_UOBJECT/RandomDialogueData.h"
#include "RandomDialogueTrigger.generated.h"


UENUM(BlueprintType)
enum class ERandomDialogueTriggerType : uint8
{
	None UMETA(Hidden),
	KillEnemy,
	KillALotOfEnemies,
	KillEnemyRage,
	KillALotOfEnemiesRage,
	LowAmmo,
	ZeroAmmo,
	PlayerGotHurt,
	KilledEnemyThatHurtYou,
	GotAPickUp
};


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API URandomDialogueTrigger : public UActorComponent, public IDialogueTriggerer
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ERandomDialogueTriggerType, TSubclassOf<URandomDialogueData>> Dialogues;
	
	protected:
	TSubclassOf<URandomDialogueData>* CurrentData;
	
public:
	URandomDialogueTrigger();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void TriggerRandomDialogue(ERandomDialogueTriggerType DialogueId);
	
	virtual void DialogueFailed_Implementation() override;
	
	protected:
	virtual void TriggerDialogue_Implementation() override;
	
	 

};
