#include "DashComponent.h"

UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

void UDashComponent::TickComponent(float DeltaTime, enum ELevelTick TickType,
                                   FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	DashUpdate(DeltaTime);
}


void UDashComponent::BeginPlay()
{
	Super::BeginPlay();
	SetComponentTickEnabled(false);
	OwnersCamera = GetOwner()->FindComponentByClass<UCameraComponent>();
	OwnersMovement = GetOwner()->FindComponentByClass<UCharacterMovementComponent>();
	OwnersSkeletalMesh = GetOwner()->FindComponentByClass<USkeletalMeshComponent>();
	OwnersHpComponent = GetOwner()->FindComponentByClass<UHpComponent>();
}

void UDashComponent::ActivateIFrames()
{
	if (OwnersHpComponent)
		OwnersHpComponent->Deactivate();
	else
		GetOwner()->SetActorEnableCollision(false);
}

void UDashComponent::PerformDash()
{
	if (!bCanDash || !IsActive())
		return;

	bCanDash = false;
	FVector OwnerVelocity = OwnersMovement->Velocity;
	OwnerVelocity.Z = 0;

	if (OwnersMovement && OwnerVelocity != FVector::ZeroVector)
	{
		DashDirection = OwnerVelocity;
		DashDirection.Normalize();
	}
	else
		DashDirection = GetOwner()->GetActorForwardVector();

	if (OwnersCamera)
		CameraStartPos = OwnersCamera->GetRelativeLocation();

	if (OwnersSkeletalMesh)
		MeshStartPos = OwnersSkeletalMesh->GetRelativeLocation();

	DashStartPos = GetOwner()->GetActorLocation();

	SetComponentTickEnabled(true);

	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);

	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UDashComponent::ActivateIFrames,
	                                       WaitTimeBeforeIFrames);
}


void UDashComponent::DashUpdate(float DeltaTime)
{
	TimePassed += DeltaTime;
	float Alpha = TimePassed / DashDuration;

	OnDashChange.Broadcast(TimePassed, DashDuration);

	if (TimePassed > DashDuration)
	{
		DashFinished();
		return;
	}

	MoveOwner(Alpha);

	LowerComponent(OwnersCamera, CameraStartPos, Alpha);
	LowerComponent(OwnersSkeletalMesh, MeshStartPos, Alpha);
}


void UDashComponent::DashFinished()
{
	LowerComponent(OwnersCamera, CameraStartPos, 1);
	LowerComponent(OwnersSkeletalMesh, MeshStartPos, 1);

	TimePassed = 0;
	SetComponentTickEnabled(false);
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UDashComponent::RefreshDash, DashCooldown);
}

void UDashComponent::RefreshDash()
{
	bCanDash = true;

	if (OwnersHpComponent)
		OwnersHpComponent->Activate();
	else
		GetOwner()->SetActorEnableCollision(true);
}

// Alpha = value between 0 and 1
void UDashComponent::LowerComponent(USceneComponent* Component, FVector StartPos, float Alpha)
{
	if (!Component)
		return;

	FVector CamRelPos = MoveVectorBasedOnCurve(StartPos, DashCamLoweringCurve, Alpha);
	Component->SetRelativeLocation(CamRelPos);
}

FVector UDashComponent::MoveVectorBasedOnCurve(FVector StartingPos, UCurveFloat* Curve, float Alpha)
{
	float CurveValue = Curve->GetFloatValue(Alpha);
	FVector CurrentPos = StartingPos;
	CurrentPos.Z += CurveValue;
	return CurrentPos;
}

void UDashComponent::Activate(bool bReset)
{
	SetActiveFlag(true);
}

void UDashComponent::Deactivate()
{
	SetActiveFlag(false);
	SetComponentTickEnabled(false);
}

void UDashComponent::MoveOwner(float Alpha)
{
	float CurveValue = DashDistanceCurve->GetFloatValue(Alpha);
	FVector CurrentPos = DashStartPos + DashDirection * CurveValue;
	GetOwner()->SetActorLocation(CurrentPos, true);
}
