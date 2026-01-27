#include "DashComponent.h"


UDashComponent::UDashComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UDashComponent::BeginPlay()
{
	Super::BeginPlay();
	FOnTimelineFloat TimelineTick;
	TimelineTick.BindUFunction(this,FName("UpdateTimeline"));
	
	FOnTimelineEvent OnTimelineFinished;
	OnTimelineFinished.BindUFunction(this,FName("TimelineFinished"));

	DashCrouchTimeline.SetPlayRate(DashCrouchTimeline.GetPlayRate()/DashDuration);
	DashCrouchTimeline.AddInterpFloat(DashCurve,TimelineTick);
	DashCrouchTimeline.SetTimelineFinishedFunc(OnTimelineFinished);
}

void UDashComponent::UpdateTimeline(float Alpha)
{
}

void UDashComponent::TimelineFinished()
{
}


