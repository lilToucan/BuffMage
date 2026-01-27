#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DashComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UDashComponent : public UActorComponent
{
	GENERATED_BODY()

	// variables
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config|Curves", meta =(ToolTip = "Only used for Player, Must be of width 1 "))
	UCurveFloat* DashCamLoweringCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config", meta =(ToolTip = "Only used for Player, Must be of width 1 "))
	UCurveFloat* DashDistanceCurve;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float DashDuration = 0.25f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float WaitTimeBeforeIFrames = 0.05f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float DashCooldown = 1;

	
protected:
	UPROPERTY(BlueprintReadWrite)
	bool bCanDash = true;

	UPROPERTY(BlueprintReadWrite)
	UCameraComponent* OwnersCamera;
	
	UPROPERTY(BlueprintReadWrite)
	UCharacterMovementComponent* OwnersMovement;
	
	UPROPERTY(BlueprintReadWrite)
	USkeletalMeshComponent* OwnersSkeletalMesh;

	FTimerHandle CooldownTimerHandle;
	FTimerHandle WaitForIFramesTimerHandle;
	float TimePassed;

	FVector DashDirection;
	FVector DashStartPos;

	FVector CameraStartPos;

	FVector MeshStartPos;

// functions
public:
	UDashComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PerformDash();
	void MoveOwner(float Alpha);
	// void LowerCamera(float Alpha);
	void LowerComponent(USceneComponent* Component,FVector StartPos,float Alpha);
	FVector MoveVectorBasedOnCurve(FVector StartingPos, UCurveFloat* Curve, float Alpha);

protected:
	virtual void BeginPlay() override;

	void ActivateIFrames();
	UFUNCTION()
	void DashUpdate(float Alpha);
	UFUNCTION()
	void DashFinished();
	UFUNCTION()
	void RefreshDash();
};
