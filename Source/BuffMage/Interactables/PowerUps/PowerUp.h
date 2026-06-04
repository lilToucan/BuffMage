#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/Interactables.h"
#include "GameFramework/Actor.h"
#include "PowerUp.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionDelegate);

UCLASS(Blueprintable,BlueprintType)
class BUFFMAGE_API APowerUp : public AActor, public IInteractables
{
	GENERATED_BODY()
// Variables
public:
	UPROPERTY(BlueprintReadWrite, BlueprintAssignable, BlueprintCallable, Category="Input|Interact")
	FOnInteractionDelegate OnPickupInteraction;

// Functions:
protected:
	virtual void BeginPlay() override;

public:
	APowerUp();
	virtual void Interact_Implementation(ABuffMageCharacter* Player) override;

	virtual void Tick(float DeltaTime) override;
};
