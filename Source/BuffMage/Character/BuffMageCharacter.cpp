#include "BuffMageCharacter.h"
#include "EnhancedInputComponent.h"

ABuffMageCharacter::ABuffMageCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackComp = CreateDefaultSubobject<UAttackComponent>("AttackComponent");
}

void ABuffMageCharacter::BeginPlay()
{
	Super::BeginPlay();

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AttackComp->AnimInstance = AnimInstance;
	if (!IsValid(AnimInstance))
		return;

	AnimInstance->OnPlayMontageNotifyBegin.AddUniqueDynamic(this, &ABuffMageCharacter::OnMontageNotifyBegin);
}

// Called every frame
void ABuffMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABuffMageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		enhancedInput->BindAction(LookAroundInputAction, ETriggerEvent::Triggered, this,
		                          &ABuffMageCharacter::AimInputFunction);

		// attack input:|
		enhancedInput->BindAction(ShootInputAction, ETriggerEvent::Started, this,
		                          &ABuffMageCharacter::AttackInputFunction);

		// move inputs :)
		enhancedInput->BindAction(MoveInputAction, ETriggerEvent::Triggered, this,
		                          &ABuffMageCharacter::MoveInputFunction);
		enhancedInput->BindAction(MoveInputAction, ETriggerEvent::Completed, this,
		                          &ABuffMageCharacter::MoveInputFunction);
	}
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

void ABuffMageCharacter::AttackInputFunction(const FInputActionValue& InputActionValue)
{
	AttackComp->Attack();
}

void ABuffMageCharacter::OnMontageNotifyBegin(FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	AttackComp->OnMontageNotifyBegin(Name, 0.4f, BranchingPointNotifyPayload);
	BP_OnMonatageNotifyBegin();
}



TObjectPtr<UAttackComponent> ABuffMageCharacter::GetAttackComponent()
{
	return AttackComp;
}
