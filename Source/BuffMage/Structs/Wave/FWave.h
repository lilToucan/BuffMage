#pragma once
#include "BuffMage/Structs/WaveData/FWaveData.h"
#include "FWave.generated.h"

USTRUCT(BlueprintType)
struct FWave
{
	GENERATED_BODY()
	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget = true))
	TArray<FWaveData> Waves;
};
