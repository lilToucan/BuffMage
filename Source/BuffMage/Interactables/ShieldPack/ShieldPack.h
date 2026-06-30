// Fill out your copyright notice in the Description page of Project Settings.#pragma once

#include "CoreMinimal.h"
#include "BuffMage/Interfaces/Pickupable/Pickupable.h"
#include "GameFramework/Actor.h"
#include "ShieldPack.generated.h"

UCLASS()
class BUFFMAGE_API AShieldPack : public AActor, public IPickupable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Config")
	float ShieldAmount;

protected:

public:
	AShieldPack();
	virtual void Tick(float DeltaTime) override;
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;
protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void BeginPlay() override;
	void DisableActor();
};
