#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DialogueTrigger.generated.h"


UCLASS(Blueprintable, BlueprintType, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UDialogueTrigger : public UActorComponent
{
	GENERATED_BODY()

//public:
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue", meta = (AllowedTypes ="FPhrase"))
	// TArray<UDataTable*> DialogueRows;
public:
	UDialogueTrigger();

	UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
	void TriggerDialogue();

protected:
	virtual void BeginPlay() override;
};
