#include "Pickup.h"

void APickup::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ABuffMageCharacter* Player = Cast<ABuffMageCharacter>(OtherActor))
	{
		PickUp_Implementation(Player);
	}
}

void APickup::PickUp_Implementation(ABuffMageCharacter* Player)
{
	DisableActor_Implementation();
}

void APickup::DisableActor_Implementation()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}
