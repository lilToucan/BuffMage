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
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ToolTip="- The image of the protagonist \n- if left empty it will show previous image"))
	TSoftObjectPtr<UTexture> MageImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite,meta=(ToolTip="- The image of the one talking to the protagonist\n- if left empty it will show nothing \n- if this is an internal dialogue of the protagonist leave empty"))
	TSoftObjectPtr<UTexture> OtherImage;
	
};
