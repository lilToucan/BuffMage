#pragma once
#include "FWaveData.generated.h"

UENUM(BlueprintType)
enum EEnemyType
{
	none,
	SkeletonMelee,
	SkeletonRanged,
	Gargoyle
};

USTRUCT(BlueprintType)
struct FWaveData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EEnemyType> EnemyType;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget = true))
	TArray<FVector> EnemyPositions;
};
