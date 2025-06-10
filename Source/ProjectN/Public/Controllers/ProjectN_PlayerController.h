// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "Input/ProjectN_InputConfig.h"
#include "Interfaces/AvatarInfoInterface.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_PlayerController.generated.h"

struct FInputActionValue;
class UProjectN_AbilitySystemComponent;
class UProjectN_DamageTextComponent;

UCLASS(Abstract)
class PROJECTN_API AProjectN_PlayerController : public APlayerController, public IAvatarInfoInterface
{
	GENERATED_BODY()

public:
	AProjectN_PlayerController();
	virtual void OnPossess(APawn* aPawn) override;

	UProjectN_AbilitySystemComponent* GetAbilitySystemComponent();

	UFUNCTION(Client, Reliable)
	void ShowDamageNumber(const float Damage, AActor* Target, const bool bBlocked, const bool bCriticalHit, const bool bEvaded);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EMovementState GetCurrentMovementMode() { return CurrentMovementMode; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FMovementData GetCurrentMovementData() { return *MovementDataMap.Find(CurrentMovementMode); }

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void SetupInputComponent() override;
	
	void OnActionPressed(FGameplayTag InputTag);
	void OnActionReleased(FGameplayTag InputTag);
	void OnActionHeld(FGameplayTag InputTag);

	void Input_Move(const FInputActionValue& ActionValue);
	void Input_Look(const FInputActionValue& ActionValue);
	void Input_AltPressed(const FInputActionValue& ActionValue);
	void Input_AltReleased(const FInputActionValue& ActionValue);
	void Input_MovementMode(const FInputActionValue& ActionValue);

	UFUNCTION(Client,Reliable)
	void ClientInitMovementMode();

	UFUNCTION(Server,Reliable)
	void ServerSetMovementData(const EMovementState NewMovementState);
	void SetMovementData(const EMovementState NewMovementState);

	/*************************
	*  Avatar Actor Interface
	**************************/
	FMovementData GetAvatarMovementData_Implementation() override;
	void UpdateMovementSpeedMultiplier_Implementation(const float NewMultiplier) override;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UProjectN_InputConfig> InputConfig;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UProjectN_DamageTextComponent> DamageTextComponentClass;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	TMap<EMovementState, FMovementData> MovementDataMap;

	UPROPERTY(Replicated)
	EMovementState CurrentMovementMode = EMovementState::Run;
	
	float CurrentMovementSpeedMultiplier = 1.f;
	
	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent;
};
