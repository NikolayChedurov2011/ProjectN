// N Chedurov All Rights Reserved

#include "ProjectN_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/InputConfig/DataAsset_InputConfig.h"
#include "Components/Input/ProjectN_InputComponent.h"
#include "ProjectN_GameplayTags.h"
#include "ProjectN_PlayerState.h"

AProjectN_PlayerCharacter::AProjectN_PlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;
}

/*
 *** Character initialize
 */
void AProjectN_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	// Init ability actor info for the Server
	InitAbilityActorInfo();
}

void AProjectN_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	// Init ability actor info for the Client
	InitAbilityActorInfo();
	//Send RPS from Client to Server when it's ready
	OnCharacterInitAbilityEnd();
}

void AProjectN_PlayerCharacter::InitAbilityActorInfo()
{
	AProjectN_PlayerState* ProjectN_PlayerState = GetPlayerState<AProjectN_PlayerState>();
	check(ProjectN_PlayerState);
	
	ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(ProjectN_PlayerState->GetAbilitySystemComponent());
	ProjectN_AttributeSet = ProjectN_PlayerState->GetAttributeSet();
	GetAbilitySystemComponent()->InitAbilityActorInfo(ProjectN_PlayerState, this);

	Super::InitAbilityActorInfo();
}

void AProjectN_PlayerCharacter::OnCharacterInitAbilityEnd_Implementation()
{
	if (HasAuthority())
	{
		GiveAbilities();
		ApplyStartupEffects();
	}
}

/*
 *
 */

/*
 *** Setup InputComponent + implement move and look functions
 */
void AProjectN_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign valid data asset"));
	
	const ULocalPlayer* LocalPLayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPLayer);

	check(EnhancedInputSubsystem);

	EnhancedInputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	UProjectN_InputComponent* ProjectNInputComponent = CastChecked<UProjectN_InputComponent>(PlayerInputComponent);

	ProjectNInputComponent->BindNativeInputAction(InputConfigDataAsset, ProjectNGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	ProjectNInputComponent->BindNativeInputAction(InputConfigDataAsset, ProjectNGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
}

void AProjectN_PlayerCharacter::Input_Move(const FInputActionValue& ActionValue)
{
	const FVector NoZVector(1.f, 1.f, 0.f);
	const FVector2d MovementVector = ActionValue.Get<FVector2d>();

	// Getting rotator
	const FRotator CameraRot = Cast<APlayerController>(Controller)->PlayerCameraManager->GetCameraRotation();
	
	// Get forward vector from rotator and normalize it
	FVector MovementForwardVector = CameraRot.Vector() * NoZVector;
	MovementForwardVector.Normalize();
	
	// Get right vector from rotator and normalize it
	FVector MovementRightVector = FRotationMatrix(CameraRot).GetScaledAxis(EAxis::Y) * NoZVector;
	MovementRightVector.Normalize();
	
	
	//const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if(MovementVector.Y != 0.f)
	{
		//const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(MovementForwardVector, MovementVector.Y);
	}

	if(MovementVector.X != 0.f)
	{
		//const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		//const FVector RightDirection = FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(MovementRightVector, MovementVector.X);
	}
}

void AProjectN_PlayerCharacter::Input_Look(const FInputActionValue& ActionValue)
{
	const FVector2d LookAxisVector = ActionValue.Get<FVector2d>();

	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if(LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
/*
 *
 */
