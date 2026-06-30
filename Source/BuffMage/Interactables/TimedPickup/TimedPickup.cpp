#include "TimedPickup.h"

ATimedPickup::ATimedPickup()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATimedPickup::PickUp_Implementation(ABuffMageCharacter* Player)
{
	Super::PickUp_Implementation(Player);
}

void ATimedPickup::DisableActor_Implementation()
{
	Super::DisableActor_Implementation();
	GetWorldTimerManager().ClearTimer(RespawnTimer);
	GetWorldTimerManager().SetTimer(RespawnTimer,this,&ATimedPickup::Respawn_Implementation, RespawnTime);
}

void ATimedPickup::Respawn_Implementation()
{
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	SetActorTickEnabled(true);
}

