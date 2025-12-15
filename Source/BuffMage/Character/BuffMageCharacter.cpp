// Fill out your copyright notice in the Description page of Project Settings.


#include "BuffMageCharacter.h"


// Sets default values
ABuffMageCharacter::ABuffMageCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ABuffMageCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuffMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABuffMageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ABuffMageCharacter::MoveInputFunction(const FInputActionValue& InputActionValue)
{
	if (!GetController()) // check if the controller exists 
		return;

	FVector2D input = InputActionValue.Get<FVector2D>(); // get the value of the input
	Velocity = input.Length();
	OnMovementInput.Broadcast(Velocity);

	// add movement to the movement component
	AddMovementInput(GetActorRightVector(), input.X);
	AddMovementInput(GetActorForwardVector(), input.Y);
}

void ABuffMageCharacter::AimInputFunction(const FInputActionValue& InputActionValue)
{
	if (!GetController())
		return;

	FVector2D input = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(input.X);
	AddControllerPitchInput(-input.Y);
}

void ABuffMageCharacter::AttackInputFunction(const FInputActionValue& InputActionValue) const
{
	OnAttackInput.Broadcast();
}

