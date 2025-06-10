// N Chedurov All Rights Reserved


#include "Controllers/ProjectN_PlayerController.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectN_GameplayTags.h"
#include "ProjectN_PlayerState.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Components/ProjectN_DamageTextComponent.h"
#include "Components/Input/ProjectN_InputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

AProjectN_PlayerController::AProjectN_PlayerController()
{
	bReplicates = true;
}

void AProjectN_PlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectN_PlayerController, CurrentMovementMode);
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

void AProjectN_PlayerController::OnPossess(APawn* aPawn)
{
	Super::OnPossess(aPawn);
	
	ClientInitMovementMode();
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
	
	ProjectNInputComponent->BindNativeInputAction(InputConfig, ProjectNGameplayTags::Input_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	ProjectNInputComponent->BindNativeInputAction(InputConfig, ProjectNGameplayTags::Input_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
	ProjectNInputComponent->BindNativeInputAction(InputConfig, ProjectNGameplayTags::Input_Alt, ETriggerEvent::Started, this, &ThisClass::Input_AltPressed);
	ProjectNInputComponent->BindNativeInputAction(InputConfig, ProjectNGameplayTags::Input_Alt, ETriggerEvent::Completed, this, &ThisClass::Input_AltReleased);
	ProjectNInputComponent->BindNativeInputAction(InputConfig, ProjectNGameplayTags::Input_MovementMode, ETriggerEvent::Started, this, &ThisClass::Input_MovementMode);
	ProjectNInputComponent->BindAbilityActions(InputConfig, this, &ThisClass::OnActionPressed, &ThisClass::OnActionReleased, &ThisClass::OnActionHeld);
}

void AProjectN_PlayerController::OnActionPressed(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectNGameplayTags::Input_Block_Action))
	{
		return;
	}
	
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->OnActionPressed(InputTag);
	}
}

void AProjectN_PlayerController::OnActionReleased(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectNGameplayTags::Input_Block_Action))
	{
		return;
	}
	
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->OnActionReleased(InputTag);
	}
}

void AProjectN_PlayerController::OnActionHeld(FGameplayTag InputTag)
{
	if (GetAbilitySystemComponent() && GetAbilitySystemComponent()->HasMatchingGameplayTag(ProjectNGameplayTags::Input_Block_Action))
	{
		return;
	}
	
	if (GetAbilitySystemComponent())
	{
		GetAbilitySystemComponent()->OnActionHeld(InputTag);
	}
}

void AProjectN_PlayerController::Input_Move(const FInputActionValue& ActionValue)
{
	if (UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetPawn())->HasMatchingGameplayTag(ProjectNGameplayTags::Input_Block_Movement))
	{
		return;	
	}
	
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

void AProjectN_PlayerController::Input_AltPressed(const FInputActionValue& ActionValue)
{
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
	
	SetShowMouseCursor(true);
}

void AProjectN_PlayerController::Input_AltReleased(const FInputActionValue& ActionValue)
{
	FInputModeGameOnly InputModeData;
	SetInputMode(InputModeData);
	
	SetShowMouseCursor(false);
}

void AProjectN_PlayerController::Input_MovementMode(const FInputActionValue& ActionValue)
{
	if (CurrentMovementMode == EMovementState::Run)
	{
		SetMovementData(EMovementState::Crouch);
		ServerSetMovementData(EMovementState::Crouch);
		return;
	}
	
	SetMovementData(EMovementState::Run);
	ServerSetMovementData(EMovementState::Run);
}

void AProjectN_PlayerController::ClientInitMovementMode_Implementation()
{
	SetMovementData(CurrentMovementMode);
}

void AProjectN_PlayerController::ServerSetMovementData_Implementation(const EMovementState NewMovementState)
{
	SetMovementData(NewMovementState);
}

void AProjectN_PlayerController::SetMovementData(const EMovementState NewMovementState)
{
	CurrentMovementMode = NewMovementState;

	ACharacter* OwningCharacter = Cast<ACharacter>(GetPawn());
	if (OwningCharacter && MovementDataMap.Find(CurrentMovementMode))
	{
		OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = MovementDataMap.Find(CurrentMovementMode)->MaxWalkSpeed * CurrentMovementSpeedMultiplier;
		OwningCharacter->GetCharacterMovement()->MaxAcceleration = MovementDataMap.Find(CurrentMovementMode)->MaxAcceleration;
		OwningCharacter->GetCharacterMovement()->BrakingDecelerationWalking = MovementDataMap.Find(CurrentMovementMode)->BrakingDeceleration;
		OwningCharacter->GetCharacterMovement()->BrakingFrictionFactor = MovementDataMap.Find(CurrentMovementMode)->BrakingFrictionFactor;
		OwningCharacter->GetCharacterMovement()->BrakingFriction = MovementDataMap.Find(CurrentMovementMode)->BrakingFriction;
		OwningCharacter->GetCharacterMovement()->bUseSeparateBrakingFriction = MovementDataMap.Find(CurrentMovementMode)->bUseSeparateBrakingFriction;
	}
}

void AProjectN_PlayerController::ShowDamageNumber_Implementation(const float Damage, AActor* Target, const bool bBlocked, const bool bCriticalHit, const bool bEvaded)
{
	if (!IsValid(Target) || !DamageTextComponentClass && IsLocalController())
	{
		return;
	}

	UProjectN_DamageTextComponent* DamageTextComponent = NewObject<UProjectN_DamageTextComponent>(Target, DamageTextComponentClass);
	DamageTextComponent->RegisterComponent();
	DamageTextComponent->AttachToComponent(Target->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	DamageTextComponent->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	DamageTextComponent->SetDamageText(Damage, bBlocked, bCriticalHit, bEvaded);
}

/*************************
*  Avatar Actor Interface
**************************/
FMovementData AProjectN_PlayerController::GetAvatarMovementData_Implementation()
{
	return *MovementDataMap.Find(CurrentMovementMode);
}

void AProjectN_PlayerController::UpdateMovementSpeedMultiplier_Implementation(const float NewMultiplier)
{
	CurrentMovementSpeedMultiplier = NewMultiplier;
}