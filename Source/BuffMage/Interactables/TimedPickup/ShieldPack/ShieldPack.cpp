#include "ShieldPack.h"

AShieldPack::AShieldPack()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AShieldPack::BeginPlay()
{
	Super::BeginPlay();
}


void AShieldPack::PickUp_Implementation(ABuffMageCharacter* Player)
{
	UHpComponent* PlayerHp = Player->FindComponentByClass<UHpComponent>();
	if (!PlayerHp)
		return;

	PlayerHp->OnHealShieldTaken(ShieldAmount, GetOwner());
	DisableActor();
}

