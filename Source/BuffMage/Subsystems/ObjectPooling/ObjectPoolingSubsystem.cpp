#include "ObjectPoolingSubsystem.h"

void UObjectPoolingSubsystem::AddPool(TSubclassOf<AActor> ActorClass, int32 InitialSize)
{
	if (!GetWorld() || !ActorClass->ImplementsInterface(UProjectilePoolable::StaticClass()))
		return;

	FObjectPool NewPool = FObjectPool();
	FActorSpawnParameters SpawnParameters;

	for (int i = 0; i < InitialSize; i++)
	{
		// spawn and add it to the pool
		AActor* newActor = GetWorld()->SpawnActor<AActor>(ActorClass, SpawnParameters); 
		IProjectilePoolable::Execute_Deactivate(newActor);
		NewPool.InactivePoolingObjects.AddUnique(TScriptInterface<IProjectilePoolable>(newActor));
	}

	if (ObjectPoolMap.Contains(ActorClass))
	{
		ObjectPoolMap[ActorClass].InactivePoolingObjects.Append(NewPool.InactivePoolingObjects);
		return;
	}

	ObjectPoolMap.Add(ActorClass, NewPool);
}

TScriptInterface<IProjectilePoolable> UObjectPoolingSubsystem::GetActor(TSubclassOf<AActor> ActorClass)
{
	if (!GetWorld() || !ActorClass->ImplementsInterface(UProjectilePoolable::StaticClass()))
		return nullptr;

	if (!ObjectPoolMap.Contains(ActorClass))
	{
		AddPool(ActorClass);
	}
	FObjectPool* PoolObject = ObjectPoolMap.Find(ActorClass);

	if (PoolObject->InactivePoolingObjects.IsEmpty())
		AddPool(ActorClass, 10);

	
	TScriptInterface<IProjectilePoolable> Result = PoolObject->InactivePoolingObjects[0];
	PoolObject->InactivePoolingObjects.Remove(Result);
	
	PoolObject->ActivePoolingObjects.AddUnique(Result);
	
	return Result;
}

void UObjectPoolingSubsystem::ReturnActorToPool(TSubclassOf<AActor> ActorClass, TScriptInterface<IProjectilePoolable> ActorToReturn)
{
	if (!ObjectPoolMap.Contains(ActorClass) || !GetWorld() || !ActorClass->ImplementsInterface(UProjectilePoolable::StaticClass()))
		return;

	FObjectPool* PoolObject = ObjectPoolMap.Find(ActorClass);

	if (!PoolObject->ActivePoolingObjects.Contains(ActorToReturn))
		return;

	PoolObject->ActivePoolingObjects.Remove(ActorToReturn);
	PoolObject->InactivePoolingObjects.AddUnique(ActorToReturn);
}
