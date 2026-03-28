#pragma once

#include "CoreMinimal.h"
#include "BuffMage/ActorComponents/HpComponent/HpComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "DashComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FRechargeBarDelegate, float, Alpha, float, MaxAmount);

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, BlueprintCallable, BlueprintAssignable, Category ="DashComponent|Delegate")
	FRechargeBarDelegate OnDashChange;

protected:
	UPROPERTY(BlueprintReadWrite, Category = "DashComponent")
	bool bCanDash = true;

	UPROPERTY(BlueprintReadWrite, Category = "DashComponent")
	UCameraComponent* OwnersCamera;

	UPROPERTY(BlueprintReadWrite, Category = "DashComponent")
	UCharacterMovementComponent* OwnersMovement;

	UPROPERTY(BlueprintReadWrite, Category = "DashComponent")
	USkeletalMeshComponent* OwnersSkeletalMesh;

	UPROPERTY(BlueprintReadWrite, Category = "DashComponent")
	UHpComponent* OwnersHpComponent;

	FTimerHandle CooldownTimerHandle;
	FTimerHandle WaitForIFramesTimerHandle;
	float TimePassed;

	FVector DashDirection;
	FVector DashStartPos;

	FVector CameraStartPos;

	FVector MeshStartPos;
	bool bIsDashing;

	// functions
public:
	UDashComponent();
	void CooldownBar( float DeltaTime);
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void PerformDash();
	void MoveOwner(float Alpha);
	// void LowerCamera(float Alpha);
	void LowerComponent(USceneComponent* Component, FVector StartPos, float Alpha);
	FVector MoveVectorBasedOnCurve(FVector StartingPos, UCurveFloat* Curve, float Alpha);

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

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
