#include "ShieldPack.h"

AShieldPack::AShieldPack()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AShieldPack::BeginPlay()
{
	Super::BeginPlay();
}

void AShieldPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AShieldPack::PickUp_Implementation(ABuffMageCharacter* Player)
{
	UHpComponent* PlayerHp = Player->FindComponentByClass<UHpComponent>();
	if (!PlayerHp)
		return;

	PlayerHp->OnHealShieldTaken(ShieldAmount, GetOwner());
	DisableActor();
}

void AShieldPack::NotifyActorBeginOverlap(AActor* OtherActor)
{
	if (ABuffMageCharacter* Player = Cast<ABuffMageCharacter>(OtherActor))
	{
		PickUp_Implementation(Player);
	}
}

void AShieldPack::DisableActor()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	Destroy();
}
