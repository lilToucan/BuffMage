#include "HpComponent.h"
#include "BuffMage/ActorComponents/Attack/AttackComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"


UHpComponent::UHpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UHpComponent::BeginPlay()
{
	Super::BeginPlay();

	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UHpComponent::OnDamageTaken);

	CharacterOwner = Cast<ACharacter>(GetOwner());
	// if (IsValid(CharacterOwner))
	// {
	// 	auto AnimInstance = CharacterOwner->GetMesh()->GetAnimInstance();
	// 	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UHpComponent::OnDeathNotify);
	// }

	Activate(true);
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


	SetActiveFlag(true);
}

void UHpComponent::Deactivate()
{
	SetActiveFlag(false);
}


bool UHpComponent::TakeDamage(float Damage, AActor* DamageCauser)
{
	if (!IsActive() || CurrentHp <= 0)
		return false;


	if (CurrentShield > 0)
	{
		CurrentShield -= Damage;
		if (CurrentShield <= 0) // if it's negative then apply the remaining damage to hp
		{
			CurrentHp += CurrentShield;
			CurrentShield = 0;
		}
		OnShieldChanged.Broadcast(CurrentShield);
	}
	else
	{
		CurrentHp -= Damage;
	}

	if (bAppliesRage && !bIsStunned)
	{
		UAttackComponent* AttackComponent = DamageCauser->GetComponentByClass<UAttackComponent>();
		if (IsValid(AttackComponent))
			AttackComponent->OnAttackHit(GetOwner());
	}

	if (CurrentHp <= 0)
	{
		CurrentHp = 0;
		OnHpChanged.Broadcast(CurrentHp);
		Death(DamageCauser);
		return false;
	}

	OnHpChanged.Broadcast(CurrentHp); // update UI

	if (HitMontage)
		CharacterOwner->PlayAnimMontage(HitMontage);

	OnHit.Broadcast();

	// play hurt sound

	if (HurtSounds.Num() <= 0)
		return true;

	USoundBase* SoundToPlay = HurtSounds[FMath::RandRange(0, HurtSounds.Num() - 1)];

	// change into normal sound
	float Volume = FMath::RandRange(HurtVolumeMinMax.X, HurtVolumeMinMax.Y);
	float Pitch = FMath::RandRange(HurtPitchMinMax.X, HurtPitchMinMax.Y);

	PlaySound(SoundToPlay, GetOwner(), Volume, Pitch);
	return true;
}

void UHpComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	TakeDamage(Damage, DamageCauser);
}

void UHpComponent::ApplyDamageDirectly(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser, bool bShouldFade)
{
	if (TakeDamage(Damage, DamageCauser))
		OnHpDirectlyChanged.Broadcast(CurrentHp); // Tell the game to go back to the checkpoint 
}

void UHpComponent::OnHealingTaken(float Healing, AActor* HealingCauser)
{
	if (!IsActive() || CurrentHp <= 0)
		return;

	CurrentHp = FMath::Min(CurrentHp + Healing, MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}

void UHpComponent::OnHealShieldTaken(float ShieldAmount, AActor* HealingCauser)
{
	if (!IsActive() || CurrentHp <= 0)
		return;

	CurrentShield = FMath::Min(CurrentShield + ShieldAmount, MaxShield);
	OnShieldChanged.Broadcast(CurrentShield);
}

void UHpComponent::Death(AActor* TheKiller)
{
	Deactivate();
	FBranchingPointNotifyPayload x;
	OnDeathStarted.Broadcast();
	if (DeathMontage)
	{
		DeathAnimDuration = CharacterOwner->PlayAnimMontage(DeathMontage);
	}
	else
		DeathAnimDuration = 1.f;

	UAttackComponent* AttackComponent = TheKiller->GetComponentByClass<UAttackComponent>();
	if (IsValid(AttackComponent))
		AttackComponent->AddToRageAfterKill();
	StartDeathTimer();

	float Volume = FMath::RandRange(DeathVolumeMinMax.X, DeathVolumeMinMax.Y);
	float Pitch = FMath::RandRange(DeathPitchMinMax.X, DeathPitchMinMax.Y);

	PlaySound(DeathSound, GetOwner(), Volume, Pitch);
}

void UHpComponent::RecoverFromStun()
{
	bIsStunned = false;
	GetOwner()->GetWorldTimerManager().ClearTimer(StunTimerHandle);
	StunTimerHandle.Invalidate();

	OnStunRecovered.Broadcast();
}

void UHpComponent::GetStunned(float Time, AActor* Instigator)
{
	if (StunTimerHandle.IsValid())
		return;

	bIsStunned = true;
	OnStunned.Broadcast();

	GetOwner()->GetWorldTimerManager().SetTimer(StunTimerHandle, this, &UHpComponent::RecoverFromStun, Time);
}

// void UHpComponent::OnDeathNotify(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
// {
// 	if (Name == DeathNotifyName)
// 	{
// 		StartDeathTimer();
// 	}
// }

void UHpComponent::PlaySound(USoundBase* CurrentSoundToPlay, AActor* HitActor, float Volume, float Pitch, USoundAttenuation* SoundAttenuation)
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

void UHpComponent::DisableOwner()
{
	OnDeathEnd.Broadcast();
}

void UHpComponent::StartDeathTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &UHpComponent::DisableOwner, DeathAnimDuration - .5f);
	DeathAnimDuration = 0;
}
