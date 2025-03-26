// N Chedurov All Rights Reserved


#include "UI/HUD/ProjectN_InGameHUD.h"

void AProjectN_InGameHUD::InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS)
{
	Super::InitOverlay(PC, PS, ASC, AS);
	OverlayWidget->SetWidgetController(OverlayWidgetController);

	BroadcastValues();
}
