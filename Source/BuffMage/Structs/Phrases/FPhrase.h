#pragma once
#include "FPhrase.generated.h"


USTRUCT(BlueprintType)
struct FPhrase 
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Phrase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;
};
