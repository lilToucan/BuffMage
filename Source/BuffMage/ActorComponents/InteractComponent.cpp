#include "InteractComponent.h"

UInteractComponent::UInteractComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UInteractComponent::Interact_Implementation(ABuffMageCharacter* Player)
{
	OnInteraction.Broadcast(Player);
}

void UInteractComponent::BeginPlay()
{
	Super::BeginPlay();

	
}

