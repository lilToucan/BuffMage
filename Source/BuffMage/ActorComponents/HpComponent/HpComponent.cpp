#include "HpComponent.h"

#include "GameFramework/Character.h"


UHpComponent::UHpComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	GetOwner()->OnTakeAnyDamage.AddUniqueDynamic(this, &UHpComponent::OnDamageTaken);
	if (StartingHP == 0)
		CurrentHp = MaxHp;
	else
		CurrentHp = StartingHP;

	if (!GetOwner())
		return;

	CharacterOwner = Cast<ACharacter>(GetOwner());
	CharacterOwner->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this,&UHpComponent::OnAnimNotifyBegin);
}

void UHpComponent::BeginPlay()
{
	Super::BeginPlay();
	if (CharacterOwner)
		return;
	CharacterOwner = Cast<ACharacter>(GetOwner());
	CharacterOwner->GetMesh()->GetAnimInstance()->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &UHpComponent::OnAnimNotifyBegin);
}

void UHpComponent::OnDamageTaken(AActor* DamagedActor, float Damage, const UDamageType* DamageType,
                                 AController* InstigatedBy, AActor* DamageCauser)
{
	CurrentHp -= Damage;

	if (CurrentHp <= 0)
		Death();
	else
	{
		CharacterOwner->PlayAnimMontage(HitMontage);
	}
}

void UHpComponent::OnHealingTaken(float Healing, AActor* HealingCauser)
{
	CurrentHp = FMath::Clamp(CurrentHp, CurrentHp + Healing, MaxHp);
}

void UHpComponent::Death()
{
	DeathAnimDuration = CharacterOwner->PlayAnimMontage(DeathMontage);
}

void UHpComponent::OnAnimNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	if (Name == DeathNotifyName)
	{
		GetWorld()->GetTimerManager().SetTimer(TimerHandle,this,&UHpComponent::DisableOwner, DeathAnimDuration);
	}
}

void UHpComponent::DisableOwner()
{
	GetOwner()->SetActorHiddenInGame(true);
	GetOwner()->SetActorEnableCollision(false);
	GetOwner()->SetActorTickEnabled(false);
}

