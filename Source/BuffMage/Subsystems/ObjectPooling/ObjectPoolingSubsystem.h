#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Structs/ObjectPool/FObjcetPool.h"
#include "Subsystems/WorldSubsystem.h"
#include "ObjectPoolingSubsystem.generated.h"

UCLASS()
class BUFFMAGE_API UObjectPoolingSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, category = "Object Pool Subsystem")
	TMap<TSubclassOf<AActor>,FObjectPool> ObjectPoolMap;
	
	UFUNCTION(BlueprintCallable, Category = "Object Pool Subsystem")
	void AddPool(TSubclassOf<AActor> ActorClass, int32 InitialSize = 50);

	UFUNCTION(BlueprintCallable, Category = "Object Pool Subsystem")
	TScriptInterface<IProjectilePoolable> GetActor(TSubclassOf<AActor> ActorClass);
	UFUNCTION(BlueprintCallable, Category = "Object Pool Subsystem")
	void ReturnActorToPool(TSubclassOf<AActor> ActorClass, TScriptInterface<IProjectilePoolable> ActorToReturn);

};
