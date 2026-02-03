#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ProjectilePoolable.generated.h"

UINTERFACE()
class UProjectilePoolable : public UInterface
{
	GENERATED_BODY()
};


class BUFFMAGE_API IProjectilePoolable
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Activate(float Velocity, float Damage, float ExplosionRange, AActor* Shooter, FVector Position, FRotator Rotation = FRotator(0.0F, 0.0F, 0.0F));
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Deactivate();
	
};
