// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/ProjectN_InputConfig.h"
#include "ProjectN_PlayerController.generated.h"

struct FInputActionValue;
class UProjectN_AbilitySystemComponent;
class UProjectN_DamageTextComponent;

UCLASS(Abstract)
class PROJECTN_API AProjectN_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectN_PlayerController();

	UProjectN_AbilitySystemComponent* GetAbilitySystemComponent();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float Damage, AActor* Target, const bool bBlocked, const bool bCriticalHit, const bool bEvaded);

protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

	void OnActionPressed(FGameplayTag InputTag);
	void OnActionReleased(FGameplayTag InputTag);
	void OnActionHeld(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& ActionValue);
	void Input_Look(const FInputActionValue& ActionValue);
	void Input_AltPressed(const FInputActionValue& ActionValue);
	void Input_AltReleased(const FInputActionValue& ActionValue);


	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UProjectN_InputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UProjectN_DamageTextComponent> DamageTextComponentClass;

	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent;
};
