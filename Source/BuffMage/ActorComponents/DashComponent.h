#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/TimelineComponent.h"
#include "DashComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()
// variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float DashDuration;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float IFrames;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float LoweredCamY;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	UCurveFloat* DashCurve;

protected:
	FTimeline DashCrouchTimeline;
	FTimerHandle IFrameHandle;
	
// functions
public:
	UDashComponent();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void UpdateTimeline(float Alpha);

	UFUNCTION()
	void TimelineFinished();
};
