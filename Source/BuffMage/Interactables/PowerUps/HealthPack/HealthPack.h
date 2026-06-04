#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interactables/PowerUps/PowerUp.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

UCLASS()
class BUFFMAGE_API AHealthPack : public APowerUp
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float HealingAmount;

protected:

public:
	AHealthPack();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(ABuffMageCharacter* Player) override;
	
protected:
	virtual void BeginPlay() override;
	void DisableActor();
};
