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

UCLASS()
class PROJECTN_API AProjectN_PlayerState : public APlayerState, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AProjectN_PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }

	FORCEINLINE int32 GetCharacterLevel() const { return CharacterLevel; }

	virtual FVector GetWeaponSocketLocation(const FGameplayTag& InputTag) override; 

protected:
	UPROPERTY(Transient)
	TObjectPtr<UAttributeSet> ProjectN_AttributeSet;

	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Replicated)
	TObjectPtr<UProjectN_InventoryComponent> ProjectN_InventoryComponent;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 CharacterLevel = 1;

	UFUNCTION()
	void OnRep_Level(int32 OldLevel);

	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const;
};