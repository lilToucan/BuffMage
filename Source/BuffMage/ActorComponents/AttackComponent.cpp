#include "AttackComponent.h"
#include "Camera/CameraComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
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
}

void UAttackComponent::SetUpWeapon_Implementation(FDynamicWeaponData& Weapon)
{
	Weapon.CurrentAmmo = Weapon.WeaponData->AmmoMax;
	Weapon.AnimIndex = 0;
	Weapon.bIsReloading = false;
	Weapon.bIsAttacking = false;
}

bool UAttackComponent::HasAmmoBeenDepleted_Implementation()
{
	if (WeaponsData[WeaponIndex].CurrentAmmo < 0)
	{
		StartReloading(); // ask designers if this should only be called with input
		return true;
	}
	return false;
}

// Called by the Owner of the component when inputting an attack
void UAttackComponent::StartAttackAnim_Implementation()
{
	/*
	// if (FireRateActive) // this bool is here for the designers TODO: REMOVE WHEN DESIGNERS DECIDE
	// {
	// 	if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].CooldownTime) //has the input been received before the fire rate was over
	// 		return;
	// }
	// else if (WeaponsData[WeaponIndex].bIsAttacking) // is it attacking?
	// 	return;
	*/

	if (GetWorld()->GetTimeSeconds() < WeaponsData[WeaponIndex].CooldownTime) //has the input been received before the fire rate was over
		return;
	if (WeaponsData[WeaponIndex].bIsAttacking) // is it attacking?
		return;
	if (WeaponsData.Num() < 1) // does this component have any weapons?
		return;
	if (!IsValid(WeaponsData[WeaponIndex].WeaponData)) // is the current weapon valid?
		return;
	if (WeaponsData[WeaponIndex].bIsReloading) // is it reloading?	
		return;

	UWeaponDataAsset* Weapon = WeaponsData[WeaponIndex].WeaponData;
	HitActors.Empty();
	// Check if you have ammo
	if (HasAmmoBeenDepleted())
		return;

	//WeaponsData[WeaponIndex].CooldownTime = GetWorld()->GetTimeSeconds() + Weapon->FireRate; // get the time the fire rate will be over (ex started attack at 4s fire rate = 3s then CooldownTime = 4s+3s = 7s)

	AnimInstance->Montage_Play(Weapon->AttackComboAnimMontage[WeaponsData[WeaponIndex].AnimIndex]); // play the animation

	// go to next Anim 
	WeaponsData[WeaponIndex].AnimIndex++;
	if (WeaponsData[WeaponIndex].AnimIndex >= Weapon->AttackComboAnimMontage.Num())
	{
		WeaponsData[WeaponIndex].AnimIndex = 0; // reset combo after completing in 
		WeaponsData[WeaponIndex].CooldownTime = GetWorld()->GetTimeSeconds() + Weapon->FireRate; // get the time the fire rate will be over (ex started attack at 4s fire rate = 3s then CooldownTime = 4s+3s = 7s)
	}

	WeaponsData[WeaponIndex].bIsAttacking = true; // set attacking to true if not using fire rate cooldown 
}

// called by the Attack notify inside the animation
void UAttackComponent::HitDetection_Implementation(FName SocketName)
{
	if (WeaponsData.Num() < 1 || // has any weapon
		!AnimInstance || // has the animation instance
		!WeaponsData[WeaponIndex].WeaponData || // the weapon is valid
		WeaponsData[WeaponIndex].bIsReloading) // is reloading
		return;

	// Check if you have ammo
	if (HasAmmoBeenDepleted())
		return;

	UWeaponDataAsset* Weapon = WeaponsData[WeaponIndex].WeaponData;

	FVector AttackPosition;

	if (!AnimInstance->GetSkelMeshComponent()->DoesSocketExist(SocketName)) // if the socket given does not exist 
		AttackPosition = GetOwner()->GetActorLocation(); // get owner position
	else
		AttackPosition = AnimInstance->GetSkelMeshComponent()->GetSocketLocation(SocketName); // get the position of the socket

	AttackPosition += GetOwner()->GetActorForwardVector() * Weapon->PositionOffsetX; // offset the position forward by PositionOffsetX

	if (Cam != nullptr) // if we have the ref to the cam
		Weapon->Attack(AttackPosition, Cam->GetComponentRotation(), GetOwner(), HitActors); // rotate attack by the cam
	else
		Weapon->Attack(AttackPosition, GetOwner()->GetActorRotation(), GetOwner(), HitActors); // rotate attack by the owner's rotation
}

// Called by the Attack Notify inside the animation
void UAttackComponent::ReduceAmmo_Implementation()
{
	WeaponsData[WeaponIndex].CurrentAmmo--;
}

// Called by the AttackCanBeUsed Notify inside the animation
void UAttackComponent::AttackCanBeUsedAgain_Implementation()
{
	WeaponsData[WeaponIndex].bIsAttacking = false;
}

// Called by the Attack Completed notify inside the animation
void UAttackComponent::AttackCompleted_Implementation()
{
	WeaponsData[WeaponIndex].AnimIndex = 0;
}

// called after performing an attack with 0 ammo or by the player's input
void UAttackComponent::StartReloading_Implementation()
{
	UWeaponDataAsset* Weapon = WeaponsData[WeaponIndex].WeaponData;
	if (WeaponsData[WeaponIndex].CurrentAmmo == Weapon->AmmoMax)
		return;

	WeaponsData[WeaponIndex].bIsAttacking = false;

	AnimInstance->StopSlotAnimation(0);
	AnimInstance->Montage_Play(Weapon->ReloadAnimMontage);
	WeaponsData[WeaponIndex].bIsReloading = true;
}

// Called by the Reload notify inside the animation
void UAttackComponent::ReloadWeapon_Implementation()
{
	WeaponsData[WeaponIndex].bIsAttacking = false;

	UWeaponDataAsset* Weapon = WeaponsData[WeaponIndex].WeaponData;
	WeaponsData[WeaponIndex].bIsReloading = false;
	WeaponsData[WeaponIndex].CurrentAmmo = Weapon->AmmoMax;
}

// Called when the owner grabs a weapon pickup
void UAttackComponent::AddWeapon_Implementation(FDynamicWeaponData& NewWeapon)
{
	if (!IsValid(NewWeapon.WeaponData))
		return;

	SetUpWeapon(NewWeapon);
	WeaponsData.Add(NewWeapon);
}

// Called by the owner of the component when switching to a different gun
void UAttackComponent::ChangeWeapon_Implementation(int InputValue)
{
	if (WeaponsData.Num() < 1 || // if there is only 1 weapon
		WeaponsData[WeaponIndex].bIsAttacking || // if its currently attacking
		WeaponsData[WeaponIndex].bIsReloading) // if its currently reloading
		return;

	if (AnimInstance)
		AnimInstance->StopAllMontages(0.2f);

	WeaponIndex += InputValue;
	if (WeaponIndex < 0)
		WeaponIndex = WeaponsData.Num() - 1; // Go to the last index
	else if (WeaponIndex >= WeaponsData.Num())
		WeaponIndex = 0; // Go to the first index
}
