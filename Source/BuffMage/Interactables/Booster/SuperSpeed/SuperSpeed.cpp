#include "SuperSpeed.h"

ASuperSpeed::ASuperSpeed()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASuperSpeed::ResetBoost()
{
	GetWorldTimerManager().ClearTimer(Timer);
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalBoostedValue;
	Destroy();
}

void ASuperSpeed::PickUp_Implementation(ABuffMageCharacter* Player)
{
	OriginalBoostedValue = PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed;
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed+= BoostAmount;
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().SetTimer(Timer, this, &ASuperSpeed::ResetBoost, Duration);
	DisableActor();
	
}

void ASuperSpeed::BeginPlay()
{
	Super::BeginPlay();
	
}

