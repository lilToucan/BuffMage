#include "HitStopComponent.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"

UHitStopComponent::UHitStopComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

void UHitStopComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);


	if (IsValid(SceneComponent))
	{
		MeshStartLocation = SceneComponent->GetRelativeLocation();
	}
	else
	{
		if (!IsValid(PlayerCharacter))
			PlayerCharacter = Cast<ACharacter>(GetOwner());

		if (IsValid(PlayerCharacter))
		{
			SceneComponent = PlayerCharacter->GetMesh();
			if (IsValid(SceneComponent))
			{
				MeshStartLocation = SceneComponent->GetRelativeLocation();
			}
		}
	}
}

void UHitStopComponent::ActivateHitStun_Implementation(float TimeDilation)
{
	if (!IsActive())
		return;

	GetOwner()->CustomTimeDilation = TimeDilation;
	GetWorld()->GetTimerManager().ClearTimer(HitStunTimer);
	GetWorld()->GetTimerManager().SetTimer(HitStunTimer, this, &UHitStopComponent::DeactivateHitStun, HitStunDuration);

	if (!bShake)
		return;

	FrameCount = 0;
	SetComponentTickEnabled(true);
}

void UHitStopComponent::DeactivateHitStun_Implementation()
{
	SetComponentTickEnabled(false);
	GetWorld()->GetTimerManager().ClearTimer(HitStunTimer);
	GetOwner()->CustomTimeDilation = 1;

	if (!bShake)
		return;

	FrameCount = 0;
	if (IsValid(SceneComponent))
		SceneComponent->SetRelativeLocation(MeshStartLocation);

	Timer = 0;
}


void UHitStopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FrameCount++;
	if (FrameCount > 0)
	{
		if (FrameCount >= FramesToStopTickFor)
			FrameCount = -1;
		else
			return;
	}

	Timer += GetWorld()->GetDeltaSeconds();

	float TimerProgress = Timer / HitStunDuration;

	float RandX = UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f);
	float RandY = UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f);

	float OffsetX = GetSinOffset(TimerProgress, RandX) * ShakeMultPerAxis.X;
	float OffsetY = GetSinOffset(TimerProgress, RandY) * ShakeMultPerAxis.Y;

	FVector ShakeVector = MeshStartLocation;
	ShakeVector.Y = MeshStartLocation.Y + OffsetY;
	ShakeVector.X = MeshStartLocation.X + OffsetX;

	if (IsValid(SceneComponent))
		SceneComponent->SetRelativeLocation(ShakeVector);
}


double UHitStopComponent::GetSinOffset(float TimerProgress, float RandNum)
{
	float ReverseProgress = 1 - TimerProgress;
	return UKismetMathLibrary::DegSin((TimerProgress * 360 * RandNum) * Frequency) * (ShakeDistance * ReverseProgress);
}

void UHitStopComponent::Activate(bool bReset)
{
	SetActiveFlag(true);
}

void UHitStopComponent::Deactivate()
{
	SetActiveFlag(false);
}
