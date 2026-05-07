#pragma once
#include "FPhrase.generated.h"


USTRUCT(BlueprintType)
struct FPhrase : public FTableRowBase
{
	GENERATED_BODY();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Phrase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ToolTip=" how long does the line take to be written"))
	float WriteTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ToolTip=" how many seconds does the line linger on screen"))
	float OnScreenTime;
};
