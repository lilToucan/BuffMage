#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitStopComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UHitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bShake = true;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector2D ShakeMultPerAxis = FVector2D(1, 1);
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float ShakeDistance;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USceneComponent* SceneComponent;
	
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float HitStunDuration = 0.2f;
	
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle HitStunTimer;

protected:
	UPROPERTY(BlueprintReadWrite)
	ACharacter* PlayerCharacter;

	UPROPERTY(BlueprintReadWrite)
	float Timer = 0;
	
	UPROPERTY(BlueprintReadWrite)
	FVector MeshStartLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Frequency = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int FramesToStopTickFor = 3;
	UPROPERTY(BlueprintReadWrite)
	int FrameCount = 0;


public:
	UHitStopComponent();
	double GetSinOffset(float TimerProgress, float RandNum);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Components")
	void ActivateHitStun(float TimeDilation = 0.f);
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Components")
	void DeactivateHitStun();

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

protected:
	virtual void BeginPlay() override;
};
