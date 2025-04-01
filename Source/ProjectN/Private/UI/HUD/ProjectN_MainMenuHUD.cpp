// N Chedurov All Rights Reserved


#include "UI/HUD/ProjectN_MainMenuHUD.h"

#include "UI/VievModel/MVVM_MainMenu.h"

void AProjectN_MainMenuHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	MainMenuViewModelPtr = NewObject<UMVVM_MainMenu>(this, MainMenuViewModelClass);
	MainMenuViewModelPtr->InitializeSaveSlots();
	
	Super::InitOverlay(PC, PS, ASC, AS);
	
	OverlayWidget->BlueprintInitializeWidget();
	MainMenuViewModelPtr->LoadData();
}
