#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/Interactables.h"
#include "Components/ActorComponent.h"
#include "InteractComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteraction, ABuffMageCharacter*, Player);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UInteractComponent : public UActorComponent, public IInteractables
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintCallable, BlueprintAssignable)
	FOnInteraction OnInteraction;

protected:
	virtual void BeginPlay() override;

public:
	UInteractComponent();
	virtual void Interact_Implementation(ABuffMageCharacter* Player) override;
};
