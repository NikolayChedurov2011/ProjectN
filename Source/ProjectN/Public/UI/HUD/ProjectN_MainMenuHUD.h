// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/HUD/ProjectN_HUD.h"
#include "ProjectN_MainMenuHUD.generated.h"

class UMVVM_MainMenu;

UCLASS()
class PROJECTN_API AProjectN_MainMenuHUD : public AProjectN_HUD
{
	GENERATED_BODY()
	
public:
	
	virtual void InitOverlay(APlayerController* PC, APlayerState* PS, UAbilitySystemComponent* ASC, UAttributeSet* AS) override;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UMVVM_MainMenu> MainMenuViewModelClass;

	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<UMVVM_MainMenu> MainMenuViewModelPtr;
	
protected:


};
