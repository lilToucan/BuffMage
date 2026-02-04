#include "AttackComponent.h"
#include "Camera/CameraComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	if (WeaponsData.Num() > 0)
		return;

	for (FDynamicWeaponData Data : WeaponsData)
	{
		if (!Data.WeaponData)
			return;
	
		Data.CurrentAmmo = Data.WeaponData->AmmoMax;
		Data.ShootTime = 0;
		Data.AnimIndex = 0;
		Data.bCanAttack = true;
	}
	
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	Cam = GetOwner()->GetComponentByClass<UCameraComponent>();

	if (WeaponsData.Num() > 0 || !WeaponsData[WeaponIndex].WeaponData)
		WeaponsData[WeaponIndex].CurrentAmmo = WeaponsData[WeaponIndex].WeaponData->AmmoMax;

	if (!AnimInstance)
		AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();
}

// Called by the Owner of the component when inputting an attack
void UAttackComponent::StartAttackAnim()
{
	if (FireRateActive)
		if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].ShootTime)
			return;
	// check if the player can attack or even has the weapon data
	if (WeaponsData.Num() < 1 || !WeaponsData[WeaponIndex].WeaponData|| !WeaponsData[WeaponIndex].bCanAttack)
		return;

	const auto Weapon = WeaponsData[WeaponIndex].WeaponData;

	WeaponsData[WeaponIndex].ShootTime = GetWorld()->GetTimeSeconds() + Weapon->FireRate;

	AnimInstance->Montage_Play(Weapon->AttackComboAnimMontage[WeaponsData[WeaponIndex].AnimIndex]);

	// go to next Anim 
	WeaponsData[WeaponIndex].AnimIndex++;
	if (WeaponsData[WeaponIndex].AnimIndex >= Weapon->AttackComboAnimMontage.Num())
		WeaponsData[WeaponIndex].AnimIndex = 0;

	if (!FireRateActive)
	WeaponsData[WeaponIndex].bCanAttack = false;
}

// called by the Attack notify inside the animation
void UAttackComponent::HitDetection(FName SocketName)
{
	if (WeaponsData.Num() < 1 || !WeaponsData[WeaponIndex].WeaponData || !AnimInstance)
		return;
	
	const auto Weapon = WeaponsData[WeaponIndex].WeaponData;

	FVector AttackPosition;

	WeaponsData[WeaponIndex].CurrentAmmo--;

	// Check Reload
	if (WeaponsData[WeaponIndex].CurrentAmmo < 0)
	{
		AnimInstance->Montage_Play(Weapon->ReloadAnimMontage); // need to move to it's own function then let input activate it
		WeaponsData[WeaponIndex].bCanAttack = false;
		return;
	}

	if (!AnimInstance->GetSkelMeshComponent()->DoesSocketExist(SocketName))
		AttackPosition = GetOwner()->GetActorLocation();
	else
		AttackPosition = AnimInstance->GetSkelMeshComponent()->GetSocketLocation(SocketName);

	AttackPosition += GetOwner()->GetActorForwardVector() * Weapon->PositionOffsetX;
	
	if (Cam != nullptr)
		Weapon->Attack(AttackPosition, Cam->GetComponentRotation(), GetOwner());
	else
		Weapon->Attack(AttackPosition, GetOwner()->GetActorRotation(), GetOwner());
}

// Called by the owner of the component when inputting a switch to a different gun
void UAttackComponent::ChangeWeapon(int InputValue)
{
	if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].ShootTime || WeaponsData.Num() < 1 || !WeaponsData[WeaponIndex].bCanAttack)
		return;
	
	WeaponIndex += InputValue;
	if (WeaponIndex < 0)
		WeaponIndex = WeaponsData.Num() - 1;
	else if (WeaponIndex >= WeaponsData.Num())
		WeaponIndex = 0;

}

// Called when the owner grabs a weapon pickup
void UAttackComponent::AddWeapon(FDynamicWeaponData& Weapon)
{
	if (Weapon.WeaponData)
		WeaponsData.Add(Weapon);
}

// Called by the Reload notify inside the animation
void UAttackComponent::ReloadWeapon()
{
	const auto Weapon = WeaponsData[WeaponIndex].WeaponData;
	WeaponsData[WeaponIndex].bCanAttack = true;
	WeaponsData[WeaponIndex].CurrentAmmo = Weapon->AmmoMax;
}

// Called by the Attack Completed notify inside the animation
void UAttackComponent::AttackCompleted()
{
	if (FireRateActive)
		return;
	
	WeaponsData[WeaponIndex].bCanAttack = true;
}
