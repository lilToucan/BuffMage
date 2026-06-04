#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RandomDialogueData.generated.h"

UCLASS(BlueprintType, Blueprintable)
class BUFFMAGE_API URandomDialogueData : public UObject
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<UDataTable*> Dialogues;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMax = 1, clampMin = 0))
	float MaxPercentage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMax = 1, clampMin = 0, ToolTip ="The percentage added after every failed trigger"))
	float PercentageAdded;
	
	UPROPERTY(BlueprintReadWrite, meta = (ClampMax = 1, clampMin = 0))
	float CurrentPercentage;
	
	UPROPERTY(BlueprintReadWrite, meta = (ClampMax = 1, clampMin = 0))
	float LastPercentage;
};
