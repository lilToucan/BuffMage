#include "Booster.h"

ABooster::ABooster()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ABooster::ResetBoost()
{
}

void ABooster::PickUp_Implementation(ABuffMageCharacter* Player)
{
	
}

void ABooster::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ABuffMageCharacter* Player = Cast<ABuffMageCharacter>(OtherActor))
	{
		PickUp_Implementation(Player);
	}
}

void ABooster::DisableActor()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}

void ABooster::BeginPlay()
{
	Super::BeginPlay();
	
}


