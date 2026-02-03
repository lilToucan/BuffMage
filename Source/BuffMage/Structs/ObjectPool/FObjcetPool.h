#pragma once

#include "BuffMage/Interfaces/Poolable/ProjectilePoolable.h"
#include "FObjcetPool.generated.h"

USTRUCT(BlueprintType)
struct FObjectPool
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "object to Pool")
	TArray<TScriptInterface<IProjectilePoolable>> ActivePoolingObjects;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "object to Pool")
	TArray<TScriptInterface<IProjectilePoolable>> InactivePoolingObjects;
};