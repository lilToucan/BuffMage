#include "AInvincibility.h"

AInvincibility::AInvincibility()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AInvincibility::ResetBoost()
{
	GetWorldTimerManager().ClearTimer(Timer);
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed = OriginalSpeed;
	PlayerCharacter->GetComponentByClass<UAttackComponent>()->ResetAddedAttackDamage();
	PlayerCharacter->GetComponentByClass<UHpComponent>()->Activate();
}

void AInvincibility::PickUp_Implementation(ABuffMageCharacter* Player)
{
	Super::PickUp_Implementation(Player);
	PlayerCharacter = Player;
	PlayerCharacter->GetComponentByClass<UAttackComponent>()->AddAttackDamageToAttacks(DamageBoost);
	PlayerCharacter->GetComponentByClass<UHpComponent>()->Deactivate();

	OriginalSpeed = PlayerCharacter->GetCharacterMovement()->GetMaxSpeed();
	PlayerCharacter->GetCharacterMovement()->MaxWalkSpeed += SpeedBoost;
	GetWorldTimerManager().ClearTimer(Timer);
	GetWorldTimerManager().SetTimer(Timer, this, &AInvincibility::ResetBoost, Duration);
}
