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

	CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	CameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	if (!GetMesh())
		return;

	FAttachmentTransformRules x = FAttachmentTransformRules::KeepRelativeTransform;
	GetMesh()->AttachToComponent(CameraComponent, x);

	CameraComponent->SetRelativeLocation(FVector(30, 0, 40));
}

void ABuffMageCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (HpComponent)
	{
		HpComponent->OnStunned.AddUniqueDynamic(this, &ABuffMageCharacter::OnStunned);
		HpComponent->OnStunRecovered.AddUniqueDynamic(this, &ABuffMageCharacter::OnStunRecovered);
	}

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (!IsValid(AnimInstance))
		return;

	AttackComp->AnimInstance = AnimInstance;
}

void ABuffMageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ABuffMageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (IsValid(EnhancedInput))
	{
		// aim input :o
		EnhancedInput->BindAction(LookAroundInputAction, ETriggerEvent::Triggered, this,
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
		                          FName("PerformDash")); // TODO: change this to a function so that you can pass if you are airborne or not
		// interact Input :l
		EnhancedInput->BindAction(InteractInputAction, ETriggerEvent::Triggered, this,
		                          &ABuffMageCharacter::InteractInputFunction);

		// Change Weapon Input :U
		EnhancedInput->BindAction(ChangeWeaponInputAction, ETriggerEvent::Triggered, this,
		                          &ABuffMageCharacter::ChangeWeaponInputFunction);
		// Reload Input :P
		EnhancedInput->BindAction(ReloadInputAction, ETriggerEvent::Started, this,
		                          &ABuffMageCharacter::ReloadInputFunction);
		// Rage Input :R
		EnhancedInput->BindAction(RageInputAction, ETriggerEvent::Started, this,
		                          &ABuffMageCharacter::RageInputFunction);
		// Jump Input :V
		EnhancedInput->BindAction(JumpInputAction, ETriggerEvent::Started, this,
		                          &ABuffMageCharacter::JumpInputFunction);
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

void ABuffMageCharacter::ChangeWeaponInputFunction(const FInputActionValue& InputActionValue)
{
	float InputValue = InputActionValue.Get<float>();
	AttackComp->ChangeWeapon(InputValue);
}

void ABuffMageCharacter::MoveInputFunction(const FInputActionValue& InputActionValue)
{
	if (!GetController()) // check if the controller exists 
		return;

	FVector2D InputValue = InputActionValue.Get<FVector2D>(); // get the value of the input
	Velocity = InputValue.Length();
	OnMovementInput.Broadcast(Velocity);

	// add movement to the movement component
	AddMovementInput(GetActorRightVector(), InputValue.X);
	AddMovementInput(GetActorForwardVector(), InputValue.Y);
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
	AttackComp->StartAttackAnim();
}

void ABuffMageCharacter::ReloadInputFunction(const FInputActionValue& InputActionValue)
{
	AttackComp->StartReloading();
}

void ABuffMageCharacter::RageInputFunction(const FInputActionValue& InputActionValue)
{
	AttackComp->StartRage();
}

void ABuffMageCharacter::JumpInputFunction(const FInputActionValue& InputActionValue)
{
	if (GetMovementComponent()->Velocity.Z == 0)
	{
		Jump();
	}
}

void ABuffMageCharacter::OnStunned()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
		return;
	DisableInput(PlayerController);
}

void ABuffMageCharacter::OnStunRecovered()
{
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController)
		return;
	EnableInput(PlayerController);
}
