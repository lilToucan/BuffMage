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
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float DashDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float DashDuration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float IFrames;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config")
	float DashCooldown;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category ="DashComponent|Config|Curves",
		meta =(ToolTip = "Only used for Player, Must be of width 1 "))
	UCurveFloat* DashCamLoweringCurve;

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
	float TimePassed;

	FVector DashDirection;
	FVector DashStartPos;
	FVector DashEndPos;

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
	FVector MoveVectorBasedOnCurve(FVector StartingPos, float Alpha);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void DashUpdate(float Alpha);
	UFUNCTION()
	void DashFinished();
	UFUNCTION()
	void RefreshDash();
};
