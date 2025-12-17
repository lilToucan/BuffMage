// Fill out your copyright notice in the Description page of Project Settings.


#include "AttackComponent.h"

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UAttackComponent::Attack()
{
	if (!IsValid(AnimInstance) || !IsValid(WeaponData->AttackComboAnimMontage))
		return;

	if (!AnimInstance->Montage_IsPlaying(WeaponData->AttackComboAnimMontage))
	{
		AnimInstance->Montage_Play(WeaponData->AttackComboAnimMontage);
	}
	else
	{
		bHasAttackBeenPerformed = true;
	}
}

void UAttackComponent::OnMontageNotifyBegin(FName Name,const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (!bHasAttackBeenPerformed)
	{
		if (IsValid(AnimInstance))
		{
			AnimInstance->Montage_Stop(0.4f,WeaponData->AttackComboAnimMontage);
		}
	}
	bHasAttackBeenPerformed = false;
}




