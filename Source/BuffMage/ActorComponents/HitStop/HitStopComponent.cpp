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

	if (!IsValid(PlayerCharacter))
		PlayerCharacter = Cast<ACharacter>(GetOwner());

	if (IsValid(PlayerCharacter))
	{
		SkeletalMeshComponent = PlayerCharacter->GetMesh();
		if (IsValid(SkeletalMeshComponent))
			MeshStartLocation = SkeletalMeshComponent->GetComponentLocation();
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

	if (IsValid(SkeletalMeshComponent))
		MeshStartLocation = SkeletalMeshComponent->GetComponentLocation();
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
	if (IsValid(SkeletalMeshComponent))
		SkeletalMeshComponent->SetWorldLocation(MeshStartLocation);

	Timer = 0;
}


void UHitStopComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FrameCount++;
	UE_LOG(LogTemp, Log, TEXT("p1p5 FrameCount = %d"), FrameCount);
	if (FrameCount > 0)
	{
		if (FrameCount >= FramesToStopTickFor)
			FrameCount = -1;
		else
			return;
	}

	Timer += GetWorld()->GetDeltaSeconds();

	float TimerProgress = Timer / HitStunDuration;

	float RandY = UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f);
	float RandX = UKismetMathLibrary::RandomFloatInRange(-1.f, 1.f);

	float OffsetY = GetSinOffset(TimerProgress, RandY);
	//									   Sin((0 to 1 value * 360°) * Frequency) * Distance
	float OffsetX = GetSinOffset(TimerProgress, RandX);

	FVector ShakeVector = MeshStartLocation;
	ShakeVector.Y = MeshStartLocation.Y + OffsetY;
	ShakeVector.X = MeshStartLocation.X + OffsetX;

	SkeletalMeshComponent->SetWorldLocation(ShakeVector);
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
