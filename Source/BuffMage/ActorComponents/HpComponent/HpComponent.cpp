#include "HpComponent.h"

#include "GameFramework/Character.h"


UHpComponent::UHpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHpComponent::Activate(bool bReset)
{
	if (bReset)
	{
		if (StartingHP == 0)
			CurrentHp = MaxHp;
		else
			CurrentHp = StartingHP;
	}

	bIsActive = true;
	
}

void UHpComponent::Deactivate()
{
	bIsActive = false;
}

void UHpComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UHpComponent::OnDamageTaken);

	CharacterOwner = Cast<ACharacter>(GetOwner());
	CharacterOwner->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UHpComponent::OnAnimNotifyBegin);

	Activate(true);
}

void UHpComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!bIsActive || CurrentHp <= 0)
		return;
	
	CurrentHp -= Damage;

	if (CurrentHp <= 0)
	{
		Death();
		return;
	}

	if (!HitMontage)
		return;
	CharacterOwner->PlayAnimMontage(HitMontage);
}

void UHpComponent::OnHealingTaken(float Healing, AActor* HealingCauser)
{
	if (!bIsActive || CurrentHp <= 0)
		return;
	
	CurrentHp = FMath::Min(CurrentHp + Healing, MaxHp);
}

void UHpComponent::RecoverFromStun()
{
	OnStunned.Broadcast();
}

void UHpComponent::GetStunned(float Time, AActor* Instigator)
{
	OnStunned.Broadcast();
	
	GetOwner()->GetWorldTimerManager().SetTimer(StunTimerHandle, this, &UHpComponent::RecoverFromStun, Time);
}

void UHpComponent::Death()
{
	if (DeathMontage)
		DeathAnimDuration = CharacterOwner->PlayAnimMontage(DeathMontage);
	StartDeathTimer();
}

void UHpComponent::StartDeathTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHpComponent::DisableOwner, DeathAnimDuration -.5f);
	DeathAnimDuration = 0;
}

void UHpComponent::OnAnimNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (Name == DeathNotifyName)
	{
		StartDeathTimer();
	}
}

void UHpComponent::DisableOwner()
{
	GetOwner()->SetActorHiddenInGame(true);
	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorTickEnabled(false);
}
