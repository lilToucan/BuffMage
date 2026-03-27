#include "AttackComponent.h"

#include "Camera/CameraComponent.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"


UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);

	GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UAttackComponent::OnDamageReceived);

	if (!IsValid(Cam))
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


	// LoadSoundAsync();
}

// SETUP WEAPON: Called at the start of the game to give default values to the WeaponAsset
void UAttackComponent::SetUpWeapon(FDynamicWeaponData& WeaponAsset)
{
	if (!IsValid(WeaponAsset.WeaponData))
		return;

	WeaponAsset.CurrentAmmo = WeaponAsset.WeaponData->AmmoMax;
	WeaponAsset.AnimIndex = 0;
	WeaponAsset.bIsReloading = false;
	WeaponAsset.bIsAttacking = false;
	// for (USoundBase* Sound : WeaponAsset.WeaponData->OnAttackHitSounds)
	// {
	// 	HitActor = GetOwner();
	// 	Volume = 0.f;
	// 	Pitch = 0.f;
	// 	LoadSoundAsync(Sound,GetOwner(),);
	// }
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
	{
		//StartReloading();
		return;
	}

	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;
	ResetHitActors();

	if (CurrentWeapon.AnimIndex >= WeaponAsset->AttackComboAnimMontage.Num())
	{
		CurrentWeapon.AnimIndex = 0; // reset combo after completing in 
		SetCooldownTime();
	}

	AnimInstance->StopAllMontages(0.1f);
	AnimInstance->Montage_Play(WeaponAsset->AttackComboAnimMontage[CurrentWeapon.AnimIndex], WeaponAsset->AnimSpeedMult); // play the animation

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
	CurrentWeapon.CooldownTime = GetWorld()->GetTimeSeconds() + WeaponAsset->CooldownBetweenCombos; // get the time the fire rate will be over (ex started attack at 4s fire rate = 3s then CooldownTime = 4s+3s = 7s)
}

// HIT DETECTION: Called by the Attack notify inside the animation
void UAttackComponent::HitDetection_Implementation(FName SocketName)
{
	if (WeaponsData.Num() < 1 || // has any weapon
		!AnimInstance || // has the animation instance
		!CurrentWeapon.WeaponData || // the weapon is valid
		CurrentWeapon.bIsReloading) // is reloading
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

// ON ATTACK HIT: Called when your attack goes through and hits an enemy :) (not called if the attack killed it D:)
void UAttackComponent::OnAttackHit_Implementation(AActor* ActorHit)
{
	USoundBase* Sound = CurrentWeapon.WeaponData->OnAttackHitSounds[FMath::RandRange(0, CurrentWeapon.WeaponData->OnAttackHitSounds.Num() - 1)];
	float Volume = FMath::RandRange(CurrentWeapon.WeaponData->VolumeMinMax.X, CurrentWeapon.WeaponData->VolumeMinMax.Y);
	float Pitch = FMath::RandRange(CurrentWeapon.WeaponData->PitchMinMax.X, CurrentWeapon.WeaponData->PitchMinMax.Y);

	PlaySound(Sound, ActorHit, Volume, Pitch, CurrentWeapon.WeaponData->SoundAttenuation);

	OnAttackHitDel.Broadcast();

	if (!bIsInRage)
		AddRage(RageAfterHitting);
}

// ADD RAGE AFTER KILL: Called after killing an opponent 
void UAttackComponent::AddToRageAfterKill_Implementation()
{
	AddRage(RageAmountAfterKilling);
}

// REDUCE AMMO: Called by the Attack Notify inside the animation
void UAttackComponent::ReduceAmmo_Implementation()
{
	CurrentWeapon.CurrentAmmo--;
	OnAmmoChange.Broadcast(CurrentWeapon.CurrentAmmo);
}

// ATTACK CAN BE USED AGAIN: Called by the AttackCanBeUsed Notify inside the animation
void UAttackComponent::AttackCanBeUsedAgain_Implementation()
{
	// go to next Anim 
	CurrentWeapon.AnimIndex++;
	CurrentWeapon.bIsAttacking = false;

	CurrentWeapon.IdleAnimIndex++;
	UpdateIdle();
}

// ATTACK COMPLETED: Called by the Attack Completed notify inside the animation reset's the combo
void UAttackComponent::AttackCompleted_Implementation()
{
	if (CurrentWeapon.bIsAttacking)
		return;
	CurrentWeapon.AnimIndex = 0;
	CurrentWeapon.IdleAnimIndex = 0;
	SetCooldownTime();
	//D! ask designers if they want to put a cooldown when you fail the combo
}

// HAS AMMO BEEN DEPLETED: Called by this component to check the ammo count of the current weapon
bool UAttackComponent::HasAmmoBeenDepleted_Implementation()
{
	if (CurrentWeapon.CurrentAmmo <= 0)
	{
		float Volume = FMath::RandRange(AmmoFinishedVolume.X, AmmoFinishedVolume.Y);
		float Pitch = FMath::RandRange(AmmoFinishedPitch.X, AmmoFinishedPitch.Y);
		PlaySound(OnAmmoFinished, GetOwner(), Volume, Pitch);
		return true;
	}
	return false;
}

// START RELOAD: called after performing an attack with 0 ammo or by the player's input
void UAttackComponent::StartReloading_Implementation()
{
	if (CurrentWeapon.bIsReloading ||
		CurrentWeapon.bIsAttacking ||
		WeaponsData.Num() < 1)
		return;

	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;

	if (!WeaponAsset)
		return;

	if (CurrentWeapon.CurrentAmmo == WeaponAsset->AmmoMax)
		return;

	AttackCompleted();
	CurrentWeapon.bIsAttacking = false;
	CurrentWeapon.IdleAnimIndex = 0;
	UpdateIdle();


	CurrentWeapon.bIsReloading = true;

	if (!AnimInstance || !WeaponAsset->ReloadAnimMontage)
	{
		ReloadWeapon();
		return;
	}

	AnimInstance->StopSlotAnimation(0);
	AnimInstance->Montage_Play(WeaponAsset->ReloadAnimMontage);
}

// RELOAD WEAPON: Called by the Reload notify inside the animation
void UAttackComponent::ReloadWeapon_Implementation()
{
	CurrentWeapon.bIsAttacking = false;

	CurrentWeapon.AnimIndex = 0;
	UWeaponDataAsset* WeaponAsset = CurrentWeapon.WeaponData;
	CurrentWeapon.bIsReloading = false;
	CurrentWeapon.CurrentAmmo = WeaponAsset->AmmoMax;
	OnAmmoChange.Broadcast(CurrentWeapon.CurrentAmmo);
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

// ON DAMAGE RECEIVED: Called when the owner has been hit by another someone
void UAttackComponent::OnDamageReceived_Implementation(AActor* Actor, float X, const UDamageType* Damage, AController* Controller, AActor* Actor1)
{
	if (!bIsInRage)
		AddRage(RageAfterGettingHit);
}

void UAttackComponent::UpdateIdle()
{
	if (CurrentWeapon.WeaponData->IdleAnim.Num() < 1)
	{
		OnIdleAnimChanged.Broadcast(nullptr);
		return;
	}

	if (CurrentWeapon.IdleAnimIndex >= CurrentWeapon.WeaponData->IdleAnim.Num())
		CurrentWeapon.IdleAnimIndex = 0;

	OnIdleAnimChanged.Broadcast(CurrentWeapon.WeaponData->IdleAnim[CurrentWeapon.IdleAnimIndex]);
}

// ON WEAPON CHANGE: Called when you change weapons
void UAttackComponent::OnCurrentWeaponChange()
{
	OnAmmoChange.Broadcast(CurrentWeapon.CurrentAmmo);
	OnWeaponIconChanged.Broadcast(CurrentWeapon.WeaponData->Icon);

	UpdateIdle();
}

// CHANGE WEAPON: Called by the owner of the component when switching to a different gun
void UAttackComponent::ChangeWeapon_Implementation(int InputValue)
{
	if (WeaponsData.Num() < 1 || // if there's 0 weapons
		CurrentWeapon.bIsAttacking || // if it's currently attacking
		CurrentWeapon.bIsReloading || // if it's currently reloading
		bIsInRage) // if it's currently in rage mode 
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
	OnCurrentWeaponChange();
}


// LOAD SOUND ASYNC: Called when you want to play a sound
void UAttackComponent::PlaySound(USoundBase* CurrentSoundToPlay, AActor* HitActor, float Volume, float Pitch, USoundAttenuation* SoundAttenuation)
{
	if (!CurrentSoundToPlay)
		return;

	UGameplayStatics::PlaySoundAtLocation(
		GetOwner(),
		CurrentSoundToPlay,
		HitActor->GetActorLocation(),
		HitActor->GetActorRotation(),
		Volume,
		Pitch,
		0,
		SoundAttenuation,
		nullptr,
		nullptr,
		nullptr
	);
}

#pragma region RAGE

// ADD RAGE: Called when an attack has been successful
void UAttackComponent::AddRage_Implementation(float Amount)
{
	SetRage(CurrentRageAmount + Amount);
}

// SET RAGE: Called every time you need to set the rage amount
void UAttackComponent::SetRage_Implementation(float RageValue)
{
	CurrentRageAmount = RageValue;

	if (CurrentRageAmount > RageMaxPoints)
	{
		CurrentRageAmount = RageMaxPoints;
		PlaySound(OnRageFullSound, GetOwner(), OnRageFullVolume, FMath::RandRange(OnRageFullPitch.X, OnRageFullPitch.Y));
	}
	else if (CurrentRageAmount < 0)
		CurrentRageAmount = 0;

	OnRageChange.Broadcast(CurrentRageAmount, RageMaxPoints);

	CurrentRageTime = RageDuration * (CurrentRageAmount / RageMaxPoints);
}

// REMOVE RAGE TIME: Called in the tick 
void UAttackComponent::RemoveRageTime_Implementation(float DeltaTime)
{
	CurrentRageTime -= DeltaTime;
	SetRage(RageMaxPoints * (CurrentRageTime / RageDuration));
}

// START RAGE: Called by the owner via input
void UAttackComponent::StartRage_Implementation()
{
	if (CurrentWeapon.bIsAttacking ||
		CurrentWeapon.bIsReloading ||
		!RageWeapon.WeaponData)
		return;

	if (bIsInRage)
	{
		StopRage();
		return;
	}

	if (CurrentRageAmount < RageMaxPoints)
		return;

	bIsInRage = true;
	WeaponsData[WeaponIndex] = CurrentWeapon;
	CurrentWeapon = RageWeapon;
	OnCurrentWeaponChange(); // update Weapon UI

	OnActivatingRage.Broadcast();

	PlaySound(StartRageSound, GetOwner(), StartRageVolume, FMath::RandRange(StartRagePitch.X, StartRagePitch.Y));
	SetComponentTickEnabled(true);
}

// STOP RAGE: Called by the rage when it gets depleated
void UAttackComponent::StopRage_Implementation()
{
	bIsInRage = false;
	RageWeapon = CurrentWeapon;
	RageWeapon.CurrentAmmo = RageWeapon.WeaponData->AmmoMax;
	RageWeapon.bIsAttacking = false;
	RageWeapon.bIsReloading = false;
	CurrentWeapon = WeaponsData[WeaponIndex];

	OnDeactivatingRage.Broadcast();

	CurrentRageTime = 0;
	if (bRemoveRageWhenStopped)
		SetRage(0);

	SetComponentTickEnabled(false);
	OnCurrentWeaponChange();
}

// TICK
void UAttackComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	if (!bIsInRage)
		return;

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentRageTime <= 0)
		StopRage();

	RemoveRageTime(DeltaTime);
}

#pragma endregion
