#include "DashComponent.h"

UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UDashComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
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
}

void UDashComponent::PerformDash()
{
	if (!bCanDash)
		return;

	bCanDash = false;

	if (OwnersMovement && OwnersMovement->Velocity != FVector::ZeroVector )
	{
		DashDirection = OwnersMovement->Velocity;
		DashDirection.Normalize();
	}
	else
		DashDirection = GetOwner()->GetActorForwardVector();

	if (OwnersCamera)
		CameraStartPos = OwnersCamera->GetRelativeLocation();
	
	if (OwnersSkeletalMesh)
		MeshStartPos = OwnersSkeletalMesh->GetRelativeLocation();

	DashStartPos = GetOwner()->GetActorLocation();
	DashEndPos = GetOwner()->GetActorLocation() + DashDirection * DashDistance;

	SetComponentTickEnabled(true);
}


void UDashComponent::DashUpdate(float DeltaTime)
{
	TimePassed += DeltaTime;
	float Alpha = TimePassed / DashDuration;

	if (TimePassed > DashDuration)
	{
		DashFinished();
		return;
	}
	
	MoveOwner(Alpha);
	
	LowerComponent(OwnersCamera,CameraStartPos,Alpha);
	LowerComponent(OwnersSkeletalMesh,MeshStartPos,Alpha);
}



void UDashComponent::DashFinished()
{
	LowerComponent(OwnersCamera,CameraStartPos,1);
	LowerComponent(OwnersSkeletalMesh,MeshStartPos,1);
	
	TimePassed = 0;
	SetComponentTickEnabled(false);
	GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(CooldownTimerHandle, this, &UDashComponent::RefreshDash, DashCooldown);
}

void UDashComponent::RefreshDash()
{
	bCanDash = true;
}
// void UDashComponent::LowerCamera(float Alpha)
// {
// 	if (!OwnersCamera)
// 		return;
// 	
// 	FVector CamRelPos = MoveVectorBasedOnCurve(CameraStartPos, Alpha);
// 	OwnersCamera->SetRelativeLocation(CamRelPos);
// }

// Alpha = value between 0 and 1
void UDashComponent::LowerComponent(USceneComponent* Component,FVector StartPos ,float Alpha)
{
	if (!Component)
		return;
	
	FVector CamRelPos = MoveVectorBasedOnCurve(StartPos, Alpha);
	Component->SetRelativeLocation(CamRelPos);
}

FVector UDashComponent::MoveVectorBasedOnCurve(FVector StartingPos, float Alpha)
{
	float CurveValue = DashCamLoweringCurve->GetFloatValue(Alpha);
	FVector CurrentPos = StartingPos;
	CurrentPos.Z += CurveValue;
	return CurrentPos;
}

void UDashComponent::MoveOwner(float Alpha)
{
	FVector CurrentPos = FMath::Lerp(DashStartPos, DashEndPos, Alpha);
	GetOwner()->SetActorLocation(CurrentPos,true);
}
