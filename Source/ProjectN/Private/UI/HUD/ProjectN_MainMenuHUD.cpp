// N Chedurov All Rights Reserved


#include "UI/HUD/ProjectN_MainMenuHUD.h"

void AProjectN_MainMenuHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	Super::InitOverlay(PC, PS, ASC, AS);
	OverlayWidget->SetWidgetController(MainMenuWidgetController);

	BroadcastValues();
}
