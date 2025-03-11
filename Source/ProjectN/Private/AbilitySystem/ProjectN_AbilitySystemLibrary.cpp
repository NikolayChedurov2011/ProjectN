// N Chedurov All Rights Reserved


#include "AbilitySystem/ProjectN_AbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ProjectN_HUD.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"


UProjectN_OverlayWidgetController* UProjectN_AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController =UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(PlayerController->GetHUD()))
		{
			AProjectN_PlayerState* PlayerState = PlayerController->GetPlayerState<AProjectN_PlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* Attributes = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetParams(PlayerController, PlayerState, AbilitySystemComponent, Attributes);

			return HUD->GetOverlayWidgetController(WidgetParams);
		}
	}
	return nullptr;
}

UProjectN_AttributeController* UProjectN_AbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController =UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(PlayerController->GetHUD()))
		{
			AProjectN_PlayerState* PlayerState = PlayerController->GetPlayerState<AProjectN_PlayerState>();
			UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
			UAttributeSet* Attributes = PlayerState->GetAttributeSet();
			const FWidgetControllerParams WidgetParams(PlayerController, PlayerState, AbilitySystemComponent, Attributes);

			return HUD->GetAttributeWidgetController(WidgetParams);
		}
	}
	return nullptr;
}
