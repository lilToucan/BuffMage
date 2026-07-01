#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interactables/TimedPickup/TimedPickup.h"
#include "AInvincibility.generated.h"

UCLASS()
class BUFFMAGE_API AInvincibility : public ATimedPickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float DamageBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float SpeedBoost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Config")
	float Duration;

protected:
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle Timer;

	UPROPERTY(BlueprintReadWrite)
	float OriginalSpeed;

	UPROPERTY(BlueprintReadWrite)
	ABuffMageCharacter* PlayerCharacter;

public:
	AInvincibility();

protected:
	void ResetBoost();
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;
};
