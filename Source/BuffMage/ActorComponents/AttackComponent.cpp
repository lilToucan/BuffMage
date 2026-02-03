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
	}
	
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	Cam = GetOwner()->GetComponentByClass<UCameraComponent>();

	if (WeaponsData.Num() > 0 || !WeaponsData[WeaponIndex].WeaponData)
		WeaponsData[WeaponIndex].CurrentAmmo = WeaponsData[WeaponIndex].WeaponData->AmmoMax;
}

void UAttackComponent::StartAttackAnim()
{
	// check if the player can attack or even has the weapon data
	if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].ShootTime || WeaponsData.Num() < 1 || !WeaponsData[WeaponIndex].WeaponData)
		return;
	const auto Weapon = WeaponsData[WeaponIndex].WeaponData;

	WeaponsData[WeaponIndex].ShootTime = GetWorld()->GetTimeSeconds() + Weapon->FireRate;

	AnimInstance->Montage_Play(Weapon->AttackComboAnimMontage[WeaponsData[WeaponIndex].AnimIndex]);

	// go to next Anim 
	WeaponsData[WeaponIndex].AnimIndex++;
	if (WeaponsData[WeaponIndex].AnimIndex >= Weapon->AttackComboAnimMontage.Num())
		WeaponsData[WeaponIndex].AnimIndex = 0;
}

void UAttackComponent::HitDetection(FName SocketName)
{
	if (WeaponsData.Num() < 1 || !WeaponsData[WeaponIndex].WeaponData)
		return;

	const auto Weapon = WeaponsData[WeaponIndex].WeaponData;

	FVector AttackPosition;

	WeaponsData[WeaponIndex].CurrentAmmo--;

	// Check Reload
	if (WeaponsData[WeaponIndex].CurrentAmmo < 0)
	{
		WeaponsData[WeaponIndex].CurrentAmmo = Weapon->AmmoMax;
		WeaponsData[WeaponIndex].ShootTime = GetWorld()->GetTimeSeconds() + Weapon->ReloadTime;
		
		AnimInstance->Montage_Play(Weapon->ReloadAnimMontage);
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

void UAttackComponent::ChangeWeapon(int InputValue)
{
	if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].ShootTime || WeaponsData.Num() < 1)
		return;
	
	WeaponIndex += InputValue;
	if (WeaponIndex < 0)
		WeaponIndex = WeaponsData.Num() - 1;
	else if (WeaponIndex >= WeaponsData.Num())
		WeaponIndex = 0;

}

void UAttackComponent::AddWeapon(FDynamicWeaponData& Weapon)
{
	if (Weapon.WeaponData)
		WeaponsData.Add(Weapon);
}
