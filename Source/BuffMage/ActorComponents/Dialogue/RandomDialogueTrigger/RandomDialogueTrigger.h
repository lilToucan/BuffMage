#pragma once

#include "CoreMinimal.h"
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
class BUFFMAGE_API URandomDialogueTrigger : public UActorComponent
{
	GENERATED_BODY()

public: 
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ERandomDialogueTriggerType, TObjectPtr<URandomDialogueData>> Dialogues;
	
public:
	URandomDialogueTrigger();
	
	UFUNCTION(BlueprintCallable, Category = "Dialogue")
	void TriggerDialogue(ERandomDialogueTriggerType DialogueId);
	
	 

};
