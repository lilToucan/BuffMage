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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(MakeEditWidget = true, ToolTip="Every position is equal to a new enemy spawned (ex 5 positions = 5 enemies of the EnemyType chosen)"))
	TArray<FVector> EnemyPositions;
};
