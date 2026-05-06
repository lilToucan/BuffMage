#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Structs/Dialogue/FDialogue.h"
#include "Components/ActorComponent.h"
#include "DialogueTrigger.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UDialogueTrigger : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Dialogue")
	TArray<UDataTable*> Dialogues;

public:
	UDialogueTrigger();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent, Category = "Dialogue")
	void TriggerDialogue();

protected:
	virtual void BeginPlay() override;
};
