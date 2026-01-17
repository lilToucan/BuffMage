// Fill out your copyright notice in the Description page of Project Settings.

#include "BuffMageController.h"
#include "EnhancedInputSubsystems.h"

void ABuffMageController::SetupInputComponent()
{
	if (!IsLocalPlayerController())
		return;

	UEnhancedInputLocalPlayerSubsystem* enhancedSubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());

	if (!enhancedSubSystem)
		return;

	for (UInputMappingContext* mappingContext : inputMappingContexts)
	{
		enhancedSubSystem->AddMappingContext(mappingContext,0);
	}
}
