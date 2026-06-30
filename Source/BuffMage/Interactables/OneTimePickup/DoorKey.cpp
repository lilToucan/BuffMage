#include "DoorKey.h"

ADoorKey::ADoorKey()
{
	PrimaryActorTick.bCanEverTick = false;
	KeyComponent = CreateDefaultSubobject<UKeyComponent>(TEXT("KeyComponent"));
}

void ADoorKey::BeginPlay()
{
	Super::BeginPlay();
	KeyComponent->OnKeyLock;	
}


void ADoorKey::PickUp_Implementation(ABuffMageCharacter* Player)
{
	KeyComponent->OnKeyUnLock.Broadcast();
	Super::PickUp_Implementation(Player);
}

void ADoorKey::DisableActor_Implementation()
{
	Destroy();
}

