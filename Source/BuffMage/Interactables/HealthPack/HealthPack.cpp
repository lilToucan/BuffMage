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

void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHealthPack::Interact_Implementation(ABuffMageCharacter* Player)
{
	UHpComponent* PlayerHp = Player->FindComponentByClass<UHpComponent>();
	if (!PlayerHp)
		return;
	
	PlayerHp->OnHealingTaken(HealingAmount,GetOwner());

	DisableActor();
}

void AHealthPack::DisableActor()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
}


