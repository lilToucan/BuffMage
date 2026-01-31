// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/KismetSystemLibrary.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttackComponent::StartAttackAnim()
{
	if (GetWorld()->GetTimeSeconds() < ShootTime || WeaponData.Num() < 1)
		// check if the player can attack or has animations
		return;

	ShootTime = GetWorld()->GetTimeSeconds() + WeaponData[WeaponIndex]->FireRate;

	AnimInstance->Montage_Play(WeaponData[WeaponIndex]->AttackComboAnimMontage[AnimIndex]);

	// go to next Anim 
	AnimIndex++;
	if (AnimIndex >= WeaponData[WeaponIndex]->AttackComboAnimMontage.Num())
		AnimIndex = 0;
}

void UAttackComponent::HitDetection(FName SocketName)
{
	auto Weapon = WeaponData[WeaponIndex];

	FVector AttackPosition;

	if (!AnimInstance->GetSkelMeshComponent()->DoesSocketExist(SocketName))
		AttackPosition = GetOwner()->GetActorLocation();
	else
		AttackPosition = AnimInstance->GetSkelMeshComponent()->GetSocketLocation(SocketName);

	AttackPosition += GetOwner()->GetActorForwardVector() * Weapon->PositionOffsetX;

	Weapon->Attack(AttackPosition, CollisionChannelsToHit, GetOwner());
}

void UAttackComponent::ChangeWeapon(int InputValue)
{
	WeaponIndex += InputValue;
	if (WeaponIndex < 0)
		WeaponIndex = WeaponData.Num() - 1;
	else if (WeaponIndex >= WeaponData.Num())
		WeaponIndex = 0;
}

void UAttackComponent::AddWeapon(UWeaponDataAsset* Weapon)
{
	WeaponData.Add(Weapon);
}
