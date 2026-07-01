// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuffMage/ActorComponents/Key/KeyComponent.h"
#include "BuffMage/Interactables/Pickup.h"
#include "DoorKey.generated.h"

UCLASS()
class BUFFMAGE_API ADoorKey : public APickup
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Door Key")
	UKeyComponent* KeyComponent;

public:
	ADoorKey();

protected:
	virtual void BeginPlay() override;
	virtual void PickUp_Implementation(ABuffMageCharacter* Player) override;
	virtual void DisableActor_Implementation() override;
};
