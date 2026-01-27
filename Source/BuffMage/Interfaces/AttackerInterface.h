#pragma once

#include "CoreMinimal.h"
#include "BuffMage/ActorComponents/AttackComponent.h"
#include "UObject/Interface.h"
#include "AttackerInterface.generated.h"

UINTERFACE()
class UAttackerInterface : public UInterface
{
	GENERATED_BODY()
};

class BUFFMAGE_API IAttackerInterface
{
	GENERATED_BODY()

public:
	virtual  TObjectPtr<UAttackComponent> GetAttackComponent() = 0;
};
