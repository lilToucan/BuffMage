#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LockComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FLockStateChanged);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API ULockComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="LockComponent|Config")
	TArray<AActor*> KeyActors;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="LockComponent")
	FLockStateChanged OnLocked;
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category="LockComponent")
	FLockStateChanged OnUnLocked;

protected:
	UPROPERTY(BlueprintReadWrite, Category="LockComponent")
	int NumOfKeys;

	UPROPERTY(BlueprintReadWrite, Category="LockComponent")
	int NumOfKeysUsed;

public:
	ULockComponent();

protected:
	UFUNCTION(BlueprintCallable)
	void KeyUnlock();
	UFUNCTION(BlueprintCallable)
	void KeyLock();
	virtual void BeginPlay() override;
};
