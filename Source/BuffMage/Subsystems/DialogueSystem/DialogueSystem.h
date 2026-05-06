#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Structs/Dialogue/FDialogue.h"
#include "Subsystems/WorldSubsystem.h"
#include "DialogueSystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNextPhraseDelegate, FString, Text);

UCLASS()
class BUFFMAGE_API UDialogueSystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintAssignable, BlueprintCallable, Category = "DialogueSystem")
	FOnNextPhraseDelegate OnNextPhrase;

	UFUNCTION(BlueprintCallable, Category = "DialogueSystem")
	void StartDialogue(/*pass an array of phrases = string text , float time*/ TArray<FDialogue> Dialogue);
};
