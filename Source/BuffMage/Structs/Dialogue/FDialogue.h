#pragma once
#include "BuffMage/Structs/Phrases/FPhrase.h"
#include "FDialogue.generated.h"

USTRUCT(BlueprintType)
struct FDialogue : public FTableRowBase	
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dialogue")
	TArray<FPhrase> Conversation;
};
