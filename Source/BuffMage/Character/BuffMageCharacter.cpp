#include "BuffMageCharacter.h"
#include "EnhancedInputComponent.h"
#include "BuffMage/Interfaces/Interactables.h"
#include "Kismet/KismetSystemLibrary.h"

ABuffMageCharacter::ABuffMageCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	AttackComp = CreateDefaultSubobject<UAttackComponent>("AttackComponent");
	DashComponent = CreateDefaultSubobject<UDashComponent>("DashComponent");
	HpComponent = CreateDefaultSubobject<UHpComponent>("HpComponent");
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

void ABuffMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABuffMageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// aim input :o
		enhancedInput->BindAction(LookAroundInputAction, ETriggerEvent::Triggered, this,
		                          &ABuffMageCharacter::AimInputFunction);

		// attack input:|
		EnhancedInput->BindAction(ShootInputAction, ETriggerEvent::Started, this,
		                          &ABuffMageCharacter::AttackInputFunction);

		// move inputs :)
		EnhancedInput->BindAction(MoveInputAction, ETriggerEvent::Triggered, this,
		                          &ABuffMageCharacter::MoveInputFunction);
		EnhancedInput->BindAction(MoveInputAction, ETriggerEvent::Completed, this,
		                          &ABuffMageCharacter::MoveInputFunction);

		// Dash Input :I
		EnhancedInput->BindAction(DashInputAction, ETriggerEvent::Completed, DashComponent,
		                          FName("PerformDash"));
		// interact Input :l
		enhancedInput->BindAction(InteractInputAction, ETriggerEvent::Triggered, this,
		                          &ABuffMageCharacter::InteractInputFunction);
	}
}

void ABuffMageCharacter::InteractInputFunction(const FInputActionValue& InputActionValue)
{
	TArray<TEnumAsByte<EObjectTypeQuery>> traceObjectTypes;
	traceObjectTypes.Add(UEngineTypes::ConvertToObjectType(InteractCollisionChannel));
	TArray<AActor*> ignoreActors;
	TArray<AActor*> outActors;
	FVector sphereSpawnLocation = GetActorLocation();
	UClass* seekClass = nullptr;
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), sphereSpawnLocation, InteractionRadius, traceObjectTypes, seekClass, ignoreActors, outActors);

	for (AActor* Actor : outActors)
	{
		if (!Actor->Implements<UInteractables>())
			continue;
		IInteractables::Execute_Interact(Actor, this);
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

void ABuffMageCharacter::OnMontageNotifyBegin(
	FName Name, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
{
	AttackComp->OnMontageNotifyBegin(Name, 0.4f, BranchingPointNotifyPayload);
	BP_OnMonatageNotifyBegin();
}


TObjectPtr<UAttackComponent> ABuffMageCharacter::GetAttackComponent()
{
	return AttackComp;
}
