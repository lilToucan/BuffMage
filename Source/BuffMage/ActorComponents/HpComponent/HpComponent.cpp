#include "HpComponent.h"

#include "BuffMage/ActorComponents/Attack/AttackComponent.h"
#include "Engine/StreamableManager.h"
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
	if (IsValid(CharacterOwner))
		CharacterOwner->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UHpComponent::OnDeathNotify);

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


void UHpComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	if (!IsActive() || CurrentHp <= 0)
		return;

	CurrentHp -= Damage;

	if (bAppliesRage)
	{
		UAttackComponent* AttackComponent = DamageCauser->GetComponentByClass<UAttackComponent>();
		if (IsValid(AttackComponent))
			AttackComponent->OnAttackHit(GetOwner());
	}

	if (CurrentHp <= 0)
	{
		Death(DamageCauser);
		return;
	}

	if (HitMontage)
		CharacterOwner->PlayAnimMontage(HitMontage);

	OnHit.Broadcast();
	if (HurtSounds.Num() > 0)
		SoundToPlay = HurtSounds[FMath::RandRange(0, HurtSounds.Num() - 1)];

	Volume = FMath::RandRange(HurtVolumeMinMax.X, HurtVolumeMinMax.Y);
	Pitch = FMath::RandRange(HurtPitchMinMax.X, HurtPitchMinMax.Y);
	FStreamableManager Streamable;
	Streamable.RequestAsyncLoad(SoundToPlay.ToSoftObjectPath(),
	                            FStreamableDelegate::CreateUObject(this, &UHpComponent::PlaySound));
}

void UHpComponent::OnHealingTaken(float Healing, AActor* HealingCauser)
{
	if (!IsActive() || CurrentHp <= 0)
		return;

	CurrentHp = FMath::Min(CurrentHp + Healing, MaxHp);
}

void UHpComponent::Death(AActor* TheKiller)
{
	Deactivate();
	OnDeathStarted.Broadcast();
	if (DeathMontage)
		DeathAnimDuration = CharacterOwner->PlayAnimMontage(DeathMontage);
	UAttackComponent* AttackComponent = TheKiller->GetComponentByClass<UAttackComponent>();
	if (IsValid(AttackComponent))
		AttackComponent->AddToRageAfterKill();
	StartDeathTimer();

	SoundToPlay = DeathSound;
	Volume = FMath::RandRange(DeathVolumeMinMax.X, DeathVolumeMinMax.Y);
	Pitch = FMath::RandRange(DeathPitchMinMax.X, DeathPitchMinMax.Y);

	FStreamableManager Streamable;
	Streamable.RequestAsyncLoad(SoundToPlay.ToSoftObjectPath(),
	                            FStreamableDelegate::CreateUObject(this, &UHpComponent::PlaySound));
}

void UHpComponent::RecoverFromStun()
{
	GetOwner()->GetWorldTimerManager().ClearTimer(StunTimerHandle);
	StunTimerHandle.Invalidate();

	OnStunRecovered.Broadcast();
}

void UHpComponent::GetStunned(float Time, AActor* Instigator)
{
	if (StunTimerHandle.IsValid())
		return;

	OnStunned.Broadcast();

	GetOwner()->GetWorldTimerManager().SetTimer(StunTimerHandle, this, &UHpComponent::RecoverFromStun, Time);
}

void UHpComponent::OnDeathNotify(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (Name == DeathNotifyName)
	{
		StartDeathTimer();
	}
}

void UHpComponent::PlaySound()
{
	USoundBase* Sound = SoundToPlay.Get();

	if (!IsValid(Sound))
		return;


	UGameplayStatics::PlaySoundAtLocation
	(
		GetOwner(),
		Sound,
		GetOwner()->GetActorLocation(),
		GetOwner()->GetActorRotation(),
		Volume,
		Pitch
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
