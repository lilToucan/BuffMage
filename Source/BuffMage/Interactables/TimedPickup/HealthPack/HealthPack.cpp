#include "HealthPack.h"

#include "BuffMage/ActorComponents/HpComponent/HpComponent.h"


AHealthPack::AHealthPack()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AHealthPack::BeginPlay()
{
	Super::BeginPlay();
}


void AHealthPack::PickUp_Implementation(ABuffMageCharacter* Player)
{
	UHpComponent* PlayerHp = Player->FindComponentByClass<UHpComponent>();
	if (!PlayerHp)
		return;

	PlayerHp->OnHealingTaken(HealingAmount, GetOwner());

	Super::PickUp_Implementation(Player); // == DisableActor_Implementation();
}

