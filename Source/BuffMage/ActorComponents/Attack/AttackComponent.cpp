#include "AttackComponent.h"
#include "Camera/CameraComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UAttackComponent::AddToRageAfterKill_Implementation()
{
	AddRage(RageAmountAfterKilling);
}

void UAttackComponent::OnAttackHit_Implementation()
{
	OnAttackHitDel.Broadcast();
	
	if (!bIsInRage)
		AddRage(RageAfterHitting);
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	
	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAttackComponent::OnDamageReceived);

	if (IsValid(Cam))
		Cam = GetOwner()->GetComponentByClass<UCameraComponent>();

	if (!AnimInstance)
		AnimInstance = GetOwner()->GetComponentByClass<USkeletalMeshComponent>()->GetAnimInstance();

	if (WeaponsData.Num() > 0)
	{
		for (FDynamicWeaponData& WeaponAsset : WeaponsData)
		{
			SetUpWeapon(WeaponAsset);
		}
		
		CurrentWeapon = WeaponsData[WeaponIndex];
	}
	SetUpWeapon(RageWeapon);
}

void UAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!bIsInRage)
		return;
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentRageAmount <= 0)
		StopRage();

	AddRage(-DeltaTime);
}

void UAttackComponent::SetUpWeapon(FDynamicWeaponData& WeaponAsset)
{

	if (!IsValid(WeaponAsset.WeaponData))
		return;
	
	WeaponAsset.CurrentAmmo = WeaponAsset.WeaponData->AmmoMax;
	WeaponAsset.AnimIndex = 0;
	WeaponAsset.bIsReloading = false;
	WeaponAsset.bIsAttacking = false;
}


// START ATTACKING: Called by the Owner of the component when inputting an attack
void UAttackComponent::StartAttackAnim_Implementation()
{
	if (GetWorld()->GetTimeSeconds() < CurrentWeapon.CooldownTime) //has the input been received before the fire rate was over
		return;
	if (CurrentWeapon.bIsAttacking) // is it attacking?
		return;
	if (WeaponsData.Num() < 1) // does this component have any weapons?
		return;
	if (!IsValid(CurrentWeapon.WeaponData)) // is the current weapon valid?
		return;
	if (CurrentWeapon.bIsReloading) // is it reloading?	
		return;
	if (!IsValid(AnimInstance))
		return;
	// Check if you have ammo
	if (HasAmmoBeenDepleted())
		return;

	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;
	ResetHitActors();

	if (CurrentWeapon.AnimIndex >= WeaponAsset->AttackComboAnimMontage.Num()) //  0 == 1 | 1 == 2 | 2 == 3 | ...
	{
		CurrentWeapon.AnimIndex = 0; // reset combo after completing in 
		SetCooldownTime();
	}
	
	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(WeaponAsset->AttackComboAnimMontage[CurrentWeapon.AnimIndex]); // play the animation

	CurrentWeapon.bIsAttacking = true; // set attacking to true if not using fire rate cooldown 
}

// RESET HIT ACTORS: Resets the current held hit actors called when you start an attack and by Notifies
void UAttackComponent::ResetHitActors_Implementation()
{
	HitActors.Empty();
}

// SET COOLDOWN TIME: Called when all anim attacks are over
void UAttackComponent::SetCooldownTime_Implementation()
{
	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;
	CurrentWeapon.CooldownTime = GetWorld()->GetTimeSeconds() + WeaponAsset->FireRate; // get the time the fire rate will be over (ex started attack at 4s fire rate = 3s then CooldownTime = 4s+3s = 7s)
}

// HIT DETECTION: Called by the Attack notify inside the animation
void UAttackComponent::HitDetection_Implementation(FName SocketName)
{
	if (WeaponsData.Num() < 1 || // has any weapon
		!AnimInstance || // has the animation instance
		!CurrentWeapon.WeaponData || // the weapon is valid
		CurrentWeapon.bIsReloading) // is reloading
		return;

	// Check if you have ammo
	if (HasAmmoBeenDepleted())
		return;

	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;

	FVector AttackPosition;

	if (!AnimInstance->GetSkelMeshComponent()->DoesSocketExist(SocketName)) // if the socket given does not exist 
		AttackPosition = GetOwner()->GetActorLocation(); // get owner position
	else
		AttackPosition = AnimInstance->GetSkelMeshComponent()->GetSocketLocation(SocketName); // get the position of the socket

	AttackPosition += GetOwner()->GetActorForwardVector() * WeaponAsset->PositionOffsetX; // offset the position forward by PositionOffsetX

	if (Cam != nullptr) // if we have the ref to the cam
		WeaponAsset->Attack(AttackPosition, Cam->GetComponentRotation(), GetOwner(), HitActors); // rotate attack by the cam
	else
		WeaponAsset->Attack(AttackPosition, GetOwner()->GetActorRotation(), GetOwner(), HitActors); // rotate attack by the owner's rotation
}

// REDUCE AMMO: Called by the Attack Notify inside the animation
void UAttackComponent::ReduceAmmo_Implementation()
{
	CurrentWeapon.CurrentAmmo--;
}

// ATTACK CAN BE USED AGAIN: Called by the AttackCanBeUsed Notify inside the animation
void UAttackComponent::AttackCanBeUsedAgain_Implementation()
{
	// go to next Anim 
	CurrentWeapon.AnimIndex++;
	CurrentWeapon.bIsAttacking = false;
}

// ATTACK COMPLETED: Called by the Attack Completed notify inside the animation reset's the combo
void UAttackComponent::AttackCompleted_Implementation()
{
	if (CurrentWeapon.bIsAttacking)
		return;
	AnimInstance->StopAllMontages(0.f);
	CurrentWeapon.AnimIndex = 0;
	SetCooldownTime();
	//D! ask designers if they want to put a cooldown when you fail the combo
}

// HAS AMMO BEEN DEPLETED: Called by this component to check the ammo count of the current weapon
bool UAttackComponent::HasAmmoBeenDepleted_Implementation()
{
	if (CurrentWeapon.CurrentAmmo < 0)
	{
		StartReloading(); // ask designers if this should only be called with input
		return true;
	}
	return false;
}

// START RELOAD: called after performing an attack with 0 ammo or by the player's input
void UAttackComponent::StartReloading_Implementation()
{
	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;
	if (CurrentWeapon.CurrentAmmo == WeaponAsset->AmmoMax)
		return;

	CurrentWeapon.bIsAttacking = false;

	AnimInstance->StopSlotAnimation(0);
	AnimInstance->Montage_Play(WeaponAsset->ReloadAnimMontage);
	CurrentWeapon.bIsReloading = true;
}

// RELOAD WEAPON: Called by the Reload notify inside the animation
void UAttackComponent::ReloadWeapon_Implementation()
{
	CurrentWeapon.bIsAttacking = false;

	CurrentWeapon.AnimIndex = 0;
	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;
	CurrentWeapon.bIsReloading = false;
	CurrentWeapon.CurrentAmmo = WeaponAsset->AmmoMax;
}

// ADD WEAPON: Called when the owner grabs a weapon pickup
void UAttackComponent::AddWeapon(FDynamicWeaponData& NewWeapon)
{
	if (!IsValid(NewWeapon.WeaponData))
		return;

	SetUpWeapon(NewWeapon);
	WeaponsData.Add(NewWeapon);
	//D! ask the designers if they want to use the weapon they picked up immediately 
}

// ADD RAGE: Called when an attack has been successful
void UAttackComponent::AddRage_Implementation(float Amount)
{
	CurrentRageAmount += Amount;
	if (CurrentRageAmount > RageDuration)
		CurrentRageAmount = RageDuration;
	else if (CurrentRageAmount < 0)
		CurrentRageAmount = 0;
	OnRageChange.Broadcast(CurrentRageAmount,RageDuration);
}

// ON DAMAGE RECEIVED: Called when the owner has been hit by another someone
void UAttackComponent::OnDamageReceived_Implementation(AActor* Actor, float X, const UDamageType* Damage, AController* Controller, AActor* Actor1)
{
	AddRage(RageAfterGettingHit);
}

// START RAGE: Called by the owner via input
void UAttackComponent::StartRage_Implementation()
{
	if (CurrentRageAmount < RageDuration)
		return;
	
	WeaponsData[WeaponIndex] = CurrentWeapon;
	CurrentWeapon = RageWeapon;
	bIsInRage = true;
	SetComponentTickEnabled(true);
}

// STOP RAGE: Called by the rage when it gets depleated
void UAttackComponent::StopRage_Implementation()
{
	RageWeapon = CurrentWeapon;
	CurrentWeapon = WeaponsData[WeaponIndex];
	bIsInRage = false;
	SetComponentTickEnabled(false);
}

// CHANGE WEAPON: Called by the owner of the component when switching to a different gun
void UAttackComponent::ChangeWeapon_Implementation(int InputValue)
{
	if (WeaponsData.Num() < 1 || // if there is only 1 weapon
		CurrentWeapon.bIsAttacking || // if its currently attacking
		CurrentWeapon.bIsReloading) // if its currently reloading
		return;

	if (AnimInstance)
		AnimInstance->StopAllMontages(0.2f);

	WeaponsData[WeaponIndex] = CurrentWeapon;

	WeaponIndex += InputValue;
	// if the index goes out of the bounds of the array then loop it 
	if (WeaponIndex < 0)
		WeaponIndex = WeaponsData.Num() - 1;
	else if (WeaponIndex >= WeaponsData.Num())
		WeaponIndex = 0;

	CurrentWeapon = WeaponsData[WeaponIndex];
}
