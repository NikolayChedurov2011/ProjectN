// N Chedurov All Rights Reserved


#include "AbilitySystem/ProjectN_AbilitySystemLibrary.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ProjectN_HUD.h"
#include "UI/WidgetController/ProjectN_WidgetControllerBase.h"


UProjectN_OverlayWidgetController* UProjectN_AbilitySystemLibrary::GetOverlayWidgetController(const UObject* WorldContextObject)
{
	if (APlayerController* PlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0))
	{
		if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(PlayerController->GetHUD()))
		{
			if (AProjectN_PlayerState* PlayerState = PlayerController->GetPlayerState<AProjectN_PlayerState>())
			{
				UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
				UAttributeSet* Attributes = PlayerState->GetAttributeSet();
				const FWidgetControllerParams WidgetParams(PlayerController, PlayerState, AbilitySystemComponent, Attributes);

				return HUD->GetOverlayWidgetController(WidgetParams);
			}
		}
	}
	return nullptr;
}

UProjectN_AttributeController* UProjectN_AbilitySystemLibrary::GetAttributeWidgetController(const UObject* WorldContextObject)
{
	APlayerController* DefaultPlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	APlayerState* DefaultPlayerState = UGameplayStatics::GetPlayerState(WorldContextObject, 0);

	if (IsValid(DefaultPlayerController) && IsValid(DefaultPlayerState))
	{
		APlayerController* PlayerController = Cast<APlayerController>(DefaultPlayerController);
		AProjectN_PlayerState* PlayerState = Cast<AProjectN_PlayerState>(DefaultPlayerState);
	
		if (IsValid(PlayerController) && IsValid(PlayerState))
		{
			if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(PlayerController->GetHUD()))
			{
				UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
				UAttributeSet* Attributes = PlayerState->GetAttributeSet();
				const FWidgetControllerParams WidgetParams(PlayerController, PlayerState, AbilitySystemComponent, Attributes);

				return HUD->GetAttributeWidgetController(WidgetParams);
			}
		}
	}
	
	return nullptr;
}

UProjectN_InventoryController* UProjectN_AbilitySystemLibrary::GetInventoryWidgetController(const UObject* WorldContextObject)
{
	APlayerController* DefaultPlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	APlayerState* DefaultPlayerState = UGameplayStatics::GetPlayerState(WorldContextObject, 0);

	if (IsValid(DefaultPlayerController) && IsValid(DefaultPlayerState))
	{
		APlayerController* PlayerController = Cast<APlayerController>(DefaultPlayerController);
		AProjectN_PlayerState* PlayerState = Cast<AProjectN_PlayerState>(DefaultPlayerState);
	
		if (IsValid(PlayerController) && IsValid(PlayerState))
		{
			if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(PlayerController->GetHUD()))
			{
				UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
				UAttributeSet* Attributes = PlayerState->GetAttributeSet();
				const FWidgetControllerParams WidgetParams(PlayerController, PlayerState, AbilitySystemComponent, Attributes);

				return HUD->GetInventoryWidgetController(WidgetParams);
			}
		}
	}
	
	return nullptr;
}

UProjectN_MainMenuWidgetController* UProjectN_AbilitySystemLibrary::GetMainMenuWidgetController(const UObject* WorldContextObject)
{
	APlayerController* DefaultPlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	APlayerState* DefaultPlayerState = UGameplayStatics::GetPlayerState(WorldContextObject, 0);

	if (IsValid(DefaultPlayerController) && IsValid(DefaultPlayerState))
	{
		APlayerController* PlayerController = Cast<APlayerController>(DefaultPlayerController);
		AProjectN_PlayerState* PlayerState = Cast<AProjectN_PlayerState>(DefaultPlayerState);
	
		if (IsValid(PlayerController) && IsValid(PlayerState))
		{
			if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(PlayerController->GetHUD()))
			{
				UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
				UAttributeSet* Attributes = PlayerState->GetAttributeSet();
				const FWidgetControllerParams WidgetParams(PlayerController, PlayerState, AbilitySystemComponent, Attributes);

				return HUD->GetMainMenuWidgetController(WidgetParams);
			}
		}
	}
	
	return nullptr;
}

UProjectN_SaveGameWidgetController* UProjectN_AbilitySystemLibrary::GetSaveGameWidgetController(const UObject* WorldContextObject)
{
	APlayerController* DefaultPlayerController = UGameplayStatics::GetPlayerController(WorldContextObject, 0);
	APlayerState* DefaultPlayerState = UGameplayStatics::GetPlayerState(WorldContextObject, 0);

	if (IsValid(DefaultPlayerController) && IsValid(DefaultPlayerState))
	{
		APlayerController* PlayerController = Cast<APlayerController>(DefaultPlayerController);
		AProjectN_PlayerState* PlayerState = Cast<AProjectN_PlayerState>(DefaultPlayerState);
	
		if (IsValid(PlayerController) && IsValid(PlayerState))
		{
			if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(PlayerController->GetHUD()))
			{
				UAbilitySystemComponent* AbilitySystemComponent = PlayerState->GetAbilitySystemComponent();
				UAttributeSet* Attributes = PlayerState->GetAttributeSet();
				const FWidgetControllerParams WidgetParams(PlayerController, PlayerState, AbilitySystemComponent, Attributes);

				return HUD->GetSaveGameWidgetController(WidgetParams);
			}
		}
	}
	
	return nullptr;
}