#include "SuperJump.h"

ASuperJump::ASuperJump()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASuperJump::BeginPlay()
{
	Super::BeginPlay();
}

void ASuperJump::ResetBoost()
{
	GetWorldTimerManager().ClearTimer(Timer);
	PlayerCharacter->GetCharacterMovement()->JumpZVelocity = OriginalBoostedValue;
	DisableActor_Implementation();
}

void ASuperJump::PickUp_Implementation(ABuffMageCharacter* Player)
{
	PlayerCharacter = Player;
	OriginalBoostedValue = Player->GetCharacterMovement()->JumpZVelocity;
	Player->GetCharacterMovement()->JumpZVelocity += BoostAmount;
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().SetTimer(Timer, this, &ASuperJump::ResetBoost, Duration);
}
