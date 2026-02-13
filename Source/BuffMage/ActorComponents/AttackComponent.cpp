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

	if (!AnimInstance)
		AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

	if (WeaponsData.Num() > 0)
	{
		for (FDynamicWeaponData& Weapon : WeaponsData)
		{
			SetUpWeapon_Implementation(Weapon);
		}
	}
	// if (WeaponsData.Num() > 0 && IsValid(WeaponsData[WeaponIndex].WeaponData))
	// 	WeaponsData[WeaponIndex].CurrentAmmo = WeaponsData[WeaponIndex].WeaponData->AmmoMax;
	// else
	// 	Deactivate();

}

// Called by the Owner of the component when inputting an attack
void UAttackComponent::StartAttackAnim_Implementation()
{
	if (FireRateActive) // this is for testing if putting a fire rate is better or not
		if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].ShootTime)
			return;
	// check if the player can attack or even has the weapon data
	if (WeaponsData.Num() < 1 || !IsValid(WeaponsData[WeaponIndex].WeaponData)|| !WeaponsData[WeaponIndex].bCanAttack)
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
void UAttackComponent::HitDetection_Implementation(FName SocketName)
{
	if (WeaponsData.Num() < 1 || !WeaponsData[WeaponIndex].WeaponData || !AnimInstance)
		return;
	
	const auto Weapon = WeaponsData[WeaponIndex].WeaponData;

	FVector AttackPosition;

	WeaponsData[WeaponIndex].CurrentAmmo--;

	// Check Reload
	if (WeaponsData[WeaponIndex].CurrentAmmo < 0)
	{
		StartReloading(Weapon);
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
void UAttackComponent::ChangeWeapon_Implementation(int InputValue)
{
	if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].ShootTime || WeaponsData.Num() < 1 || !WeaponsData[WeaponIndex].bCanAttack)
		return;
	
	WeaponIndex += InputValue;
	if (WeaponIndex < 0)
		WeaponIndex = WeaponsData.Num() - 1;
	else if (WeaponIndex >= WeaponsData.Num())
		WeaponIndex = 0;

}

void UAttackComponent::SetUpWeapon_Implementation(FDynamicWeaponData& Weapon)
{
	Weapon.CurrentAmmo = Weapon.WeaponData->AmmoMax;
	Weapon.AnimIndex = 0;
	Weapon.bCanAttack = true;
}

// Called when the owner grabs a weapon pickup
void UAttackComponent::AddWeapon_Implementation(FDynamicWeaponData& Weapon)
{
	if (!IsValid(Weapon.WeaponData))
		return;

	SetUpWeapon(Weapon);
	WeaponsData.Add(Weapon);
}

// called after performing an attack with 0 ammo or by the player's input
void UAttackComponent::StartReloading_Implementation(UWeaponDataAsset* const Weapon)
{
	AnimInstance->Montage_Play(Weapon->ReloadAnimMontage); // need to move to it's own function then let input activate it
	WeaponsData[WeaponIndex].bCanAttack = false;
}

// Called by the Reload notify inside the animation
void UAttackComponent::ReloadWeapon_Implementation()
{
	const auto Weapon = WeaponsData[WeaponIndex].WeaponData;
	WeaponsData[WeaponIndex].bCanAttack = true;
	WeaponsData[WeaponIndex].CurrentAmmo = Weapon->AmmoMax;
}

// Called by the Attack Completed notify inside the animation
void UAttackComponent::AttackCompleted_Implementation()
{
	if (FireRateActive)
		return;
	
	WeaponsData[WeaponIndex].bCanAttack = true;
}
