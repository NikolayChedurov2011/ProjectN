// N Chedurov All Rights Reserved


#include "Controllers/ProjectN_PlayerController.h"

AProjectN_PlayerController::AProjectN_PlayerController()
{
	bReplicates = true;
}

void AProjectN_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
	
	FInputModeGameAndUI InputModeData;
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputModeData.SetHideCursorDuringCapture(false);
	SetInputMode(InputModeData);
}
