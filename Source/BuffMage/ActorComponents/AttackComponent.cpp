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

void UAttackComponent::DealDamage()
{
	TArray<AActor*> hitActors = AttackOverlapSphere();
	
	if (hitActors.Num() <= 0)
		return;

	for (auto actor : hitActors)
	{
		FDamageEvent DamageEvent;
		actor->TakeDamage(WeaponData->Damage,DamageEvent,nullptr,GetOwner());
	};
}

void UAttackComponent::CheckComboPerformed(float BlendTime)
{
	// check combo
	if (!bHasAttackBeenPerformed) // if the player didn't perform the attack input before this notify then stop the combo 
	{
		if (IsValid(AnimInstance))
		{
			AnimInstance->Montage_Stop(BlendTime,WeaponData->AttackComboAnimMontage);
		}
	}
	bHasAttackBeenPerformed = false;
}

void UAttackComponent::OnMontageNotifyBegin(FName Name,float BlendTime,const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	CheckComboPerformed(BlendTime);
	DealDamage();
}

TArray<AActor*> UAttackComponent::AttackOverlapSphere()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> EObjectTypeQueryArray;
	EObjectTypeQueryArray.Add(UEngineTypes::ConvertToObjectType(CollisionChannelToHit));
	UClass* actorTypeFilters = nullptr;
	TArray<AActor*> ignoreActors;
	ignoreActors.Add(GetOwner());
	TArray<AActor*> outActors;

	
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetOwner()->GetActorLocation(), OverlapSphereRange, EObjectTypeQueryArray,
											  actorTypeFilters, ignoreActors, outActors);

	return outActors;
}






