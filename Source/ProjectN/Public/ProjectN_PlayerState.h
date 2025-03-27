// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/CombatInterface.h"
#include "ProjectN_PlayerState.generated.h"

struct FOnAttributeChangeData;
class UProjectN_AbilitySystemComponent;
class UAttributeSet;
class UProjectN_InventoryComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameplayValueChangedSignature, int32 /*Value*/);

UCLASS()
class PROJECTN_API AProjectN_PlayerState : public APlayerState, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AProjectN_PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }

	FORCEINLINE int32 GetCharacterLevel() const { return Level; }

	virtual FVector GetWeaponSocketLocation(const FGameplayTag& InputTag) override;
	
	FOnGameplayValueChangedSignature OnLevelChanged;
	FOnGameplayValueChangedSignature OnXPChanged;
	FOnGameplayValueChangedSignature OnAttributePointsChanged;

	FORCEINLINE void SetLevel(int32 NewLevel);
	FORCEINLINE void AddToLevel(int32 NewLevel);
	FORCEINLINE int32 GetLevel() const { return Level; }

	FORCEINLINE void SetXP(int32 NewXP);
	FORCEINLINE void AddToXP(int32 NewXP);
	FORCEINLINE int32 GetXP() const { return XP; }

	FORCEINLINE void SetAttributePoints(int32 NewAttributePoints);
	FORCEINLINE void AddToAttributePoints(int32 NewAttributePoints);
	FORCEINLINE int32 GetAttributePoints() const { return AttributePoints; }

protected:
	UPROPERTY(Transient)
	TObjectPtr<UAttributeSet> ProjectN_AttributeSet;

	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Replicated)
	TObjectPtr<UProjectN_InventoryComponent> ProjectN_InventoryComponent;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 AttributePoints = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);
	UFUNCTION()
	void OnRep_XP(int32 OldXP);
	UFUNCTION()
	void OnRep_AttributePoints(int32 OldAttributePoints);

	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const;
};