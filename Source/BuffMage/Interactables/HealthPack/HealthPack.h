#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/Interactables.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

UCLASS()
class BUFFMAGE_API AHealthPack : public AActor, public IInteractables
{
	GENERATED_BODY()

public:

protected:

public:
	AHealthPack();
	virtual void Tick(float DeltaTime) override;
	virtual void Interact_Implementation(ABuffMageCharacter* Player) override;
	
protected:
	virtual void BeginPlay() override;
};
