#include "SuperSpeed.h"

ASuperSpeed::ASuperSpeed()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ASuperSpeed::ResetBoost()
{
	GetWorldTimerManager().ClearTimer(Timer);
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalBoostedValue;

}

void ASuperSpeed::PickUp_Implementation(ABuffMageCharacter* Player)
{
	PlayerCharacter = Player;
	
	OriginalBoostedValue = PlayerCharacter->GetCharacterMovement()->GetMaxSpeed();
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed+= BoostAmount;
	DisableActor_Implementation();
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().SetTimer(Timer, this, &ASuperSpeed::ResetBoost, Duration);
	
}

void ASuperSpeed::BeginPlay()
{
	Super::BeginPlay();
	
}

