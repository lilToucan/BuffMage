// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"

#include "GameFramework/PlayerController.h"
#include "BuffMageController.generated.h"

/**
 * 
 */
UCLASS()
class BUFFMAGE_API ABuffMageController : public APlayerController
{
	GENERATED_BODY()

	protected:
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Category="Inputs")
	TArray<UInputMappingContext*> inputMappingContexts;
	
	virtual void SetupInputComponent() override;
	
};
