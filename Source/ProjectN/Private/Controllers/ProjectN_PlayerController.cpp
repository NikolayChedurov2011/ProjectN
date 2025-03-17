// N Chedurov All Rights Reserved


#include "Controllers/ProjectN_PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectN_GameplayTags.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "Components/Input/ProjectN_InputComponent.h"

AProjectN_PlayerController::AProjectN_PlayerController()
{
	bReplicates = true;
}

void AProjectN_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = false;
	//DefaultMouseCursor = EMouseCursor::Default;
	
	//FInputModeGameAndUI InputModeData;
	//InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	//InputModeData.SetHideCursorDuringCapture(false);
	//SetInputMode(InputModeData);
}

UProjectN_AbilitySystemComponent* AProjectN_PlayerController::GetAbilitySystemComponent()
{
	if (ProjectN_AbilitySystemComponent == nullptr)
	{
		ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn()));
	}
	return ProjectN_AbilitySystemComponent;
}

void AProjectN_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	checkf(InputConfig, TEXT("Forgot to assign valid data asset, please fill out Character data"));
	
	const ULocalPlayer* LocalPLayer = GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPLayer);
	UProjectN_InputComponent* ProjectNInputComponent = CastChecked<UProjectN_InputComponent>(InputComponent);

	if (EnhancedInputSubsystem == nullptr)
	{
		return;
	}
	EnhancedInputSubsystem->AddMappingContext(InputConfig->DefaultMappingContext, 0);
	
	ProjectNInputComponent->BindNativeInputAction(InputConfig, ProjectNGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	ProjectNInputComponent->BindNativeInputAction(InputConfig, ProjectNGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	ProjectNInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::OnActionPressed, &ThisClass::OnActionReleased, &ThisClass::OnActionHeld);
}

void AProjectN_PlayerController::OnActionPressed(FGameplayTag InputTag)
{

}

void AProjectN_PlayerController::OnActionReleased(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->OnActionReleased(InputTag);
	}
}

void AProjectN_PlayerController::OnActionHeld(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->OnActionHeld(InputTag);
	}
}

void AProjectN_PlayerController::Input_Move(const FInputActionValue& ActionValue)
{
	const FVector NoZVector(1.f, 1.f, 0.f);
	const FVector2d MovementVector = ActionValue.Get<FVector2d>();

	// Getting rotator
	const FRotator CameraRot = PlayerCameraManager->GetCameraRotation();
	
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

		GetPawn()->AddMovementInput(MovementForwardVector, MovementVector.Y);
	}

	if(MovementVector.X != 0.f)
	{
		//const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		//const FVector RightDirection = FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::Y);

		GetPawn()->AddMovementInput(MovementRightVector, MovementVector.X);
	}
}

void AProjectN_PlayerController::Input_Look(const FInputActionValue& ActionValue)
{
	const FVector2d LookAxisVector = ActionValue.Get<FVector2d>();

	if(LookAxisVector.X != 0.f)
	{
		GetPawn()->AddControllerYawInput(LookAxisVector.X);
	}

	if(LookAxisVector.Y != 0.f)
	{
		GetPawn()->AddControllerPitchInput(LookAxisVector.Y);
	}
}
