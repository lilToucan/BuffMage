#include "LockComponent.h"

#include "BuffMage/ActorComponents/Key/KeyComponent.h"

ULockComponent::ULockComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void ULockComponent::BeginPlay()
{
	Super::BeginPlay();
	for (AActor* Actor : KeyActors)
	{
		UKeyComponent* KeyComp = Actor->GetComponentByClass<UKeyComponent>();

		if (!KeyComp)
			continue;

		KeyComp->OnKeyUnLock.AddUniqueDynamic(this,&ULockComponent::KeyUnlock);
		KeyComp->OnKeyLock.AddUniqueDynamic(this,&ULockComponent::KeyLock);
		NumOfKeys++;
	}
}

void ULockComponent::KeyUnlock()
{
	NumOfKeysUsed++;
	if (NumOfKeysUsed >= NumOfKeys)
	{
		OnUnLocked.Broadcast();
	}
}

void ULockComponent::KeyLock()
{
	NumOfKeysUsed--;
	if (NumOfKeysUsed < NumOfKeys)
	{
		OnLocked.Broadcast();
	}
}
