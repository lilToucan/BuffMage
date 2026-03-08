#include "HitStopComponent.h"

UHitStopComponent::UHitStopComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UHitStopComponent::BeginPlay()
{
	Super::BeginPlay();
	GetOwner()->OnTakeAnyDamage.AddDynamic(this,&UHitStopComponent::OnTakeDamage);
}

void UHitStopComponent::OnTakeDamage_Implementation(AActor* Actor, float Damage, const UDamageType* DamageType, AController* Controller, AActor* Instigator)
{
	if (!IsActive())
		return;
	ActivateHitStun();
}


void UHitStopComponent::ActivateHitStun_Implementation()
{
	if (!IsActive())
		return;
	
	GetOwner()->CustomTimeDilation = 0;
	GetWorld()->GetTimerManager().ClearTimer(HitStunTimer);
	GetWorld()->GetTimerManager().SetTimer(HitStunTimer,this,&UHitStopComponent::DeactivateHitStun,HitStunDuration);
}

void UHitStopComponent::DeactivateHitStun_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(HitStunTimer);
	GetOwner()->CustomTimeDilation = 1;
}

void UHitStopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UHitStopComponent::Activate(bool bReset)
{
	SetActiveFlag(true);
}

void UHitStopComponent::Deactivate()
{
	SetActiveFlag(false);
}

