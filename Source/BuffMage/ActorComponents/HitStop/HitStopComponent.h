#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HitStopComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class BUFFMAGE_API UHitStopComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle HitStunTimer;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float HitStunDuration = 0.2f;



public:
	UHitStopComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Components")
	void ActivateHitStun();
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Components")
	void DeactivateHitStun();

	virtual void Activate(bool bReset = false) override;
	virtual void Deactivate() override;

protected:
	UFUNCTION(BlueprintNativeEvent,BlueprintCallable, Category = "Components")
	void OnTakeDamage(AActor* Actor, float Damage, const UDamageType* DamageType, AController* Controller, AActor* Instigator);
	virtual void BeginPlay() override;
};
