// N Chedurov All Rights Reserved


#include "Controllers/ProjectN_PlayerController.h"

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

void AProjectN_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	//UProjectN_InputComponent* EnhancedInputComponent = CastChecked<UProjectN_InputComponent>(InputComponent);
}

void AProjectN_PlayerController::OnActionPressed(FGameplayTag InTag)
{
	
}

void AProjectN_PlayerController::OnActionCanceled(FGameplayTag InTag)
{
	
}

void AProjectN_PlayerController::OnActionHeld(FGameplayTag InTag)
{
	
}
