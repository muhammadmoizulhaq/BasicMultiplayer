// Copyright Epic Games, Inc. All Rights Reserved.

#include "BasicMultiplayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Components/WidgetComponent.h"
#include "Kismet/KismetStringLibrary.h"
#include "NamePlate.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ABasicMultiplayerCharacter::ABasicMultiplayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 230.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Widget Component for NamePlate
	Nameplate = CreateDefaultSubobject<UWidgetComponent>(TEXT("Nameplate"));
	Nameplate->SetupAttachment(GetMesh(), UKismetStringLibrary::Conv_StringToName("head"));
	Nameplate->SetRelativeLocation(FVector(45.f, 0.f, 0.f));
	Nameplate->SetRelativeRotation(FRotator(0.f, -90.f, 90.f));
	Nameplate->SetWidgetSpace(EWidgetSpace::Screen);
	Nameplate->bHiddenInGame = true;

	WalkSpeed = 230.f;
	SprintSpeed = 500.f;

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void ABasicMultiplayerCharacter::NameSet(const FText& Name)
{
	if (UNamePlate* NP = Cast<UNamePlate>(Nameplate->GetWidget()))
	{
		NP->SetName(Name);
		Nameplate->bHiddenInGame = false;
		UE_LOG(LogTemp, Warning, TEXT("Player Name {Character}: [%s]"), *Name.ToString());
	}
}

void ABasicMultiplayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void ABasicMultiplayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void ABasicMultiplayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Sprint
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ABasicMultiplayerCharacter::Sprint);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ABasicMultiplayerCharacter::StopSprint);

		// Interact
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ABasicMultiplayerCharacter::Interact);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ABasicMultiplayerCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ABasicMultiplayerCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ABasicMultiplayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void ABasicMultiplayerCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

// Sprint Setup with validation

void ABasicMultiplayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 230.f;
	Server_Walk(GetCharacterMovement()->MaxWalkSpeed);
}

void ABasicMultiplayerCharacter::Sprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	Server_Sprint(GetCharacterMovement()->MaxWalkSpeed);
}

void ABasicMultiplayerCharacter::Interact()
{
	Server_Interact();
}

void ABasicMultiplayerCharacter::Server_Interact_Implementation()
{
}

bool ABasicMultiplayerCharacter::Server_Walk_Validate(const float& Speed)
{
	if (Speed != WalkSpeed)
	{
		return false;
	}
	return true;
}

bool ABasicMultiplayerCharacter::Server_Sprint_Validate(const float& Speed)
{
	if (Speed != SprintSpeed)
	{
		return false;
	}
	return true;
}

void ABasicMultiplayerCharacter::Server_Walk_Implementation(const float& Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ABasicMultiplayerCharacter::Server_Sprint_Implementation(const float& Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}