#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "KeyComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnKeyUsed);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UKeyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite,BlueprintAssignable,BlueprintCallable,Category="KeyComponent|Config")
	FOnKeyUsed OnKeyUnLock;
	UPROPERTY(BlueprintReadWrite,BlueprintAssignable,BlueprintCallable,Category="KeyComponent|Config")
	FOnKeyUsed OnKeyLock;
	
};
