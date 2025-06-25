// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectN_GameplayAbilityBase.generated.h"


struct FEntriesDefinition;
enum class EEquipSlot : uint8;

UCLASS()
class PROJECTN_API UProjectN_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:

	FORCEINLINE const FGameplayTag& GetStartupTag() const { return StartupTag; }
	FORCEINLINE FName GetItemId() const { return AbilityID; }

protected:

	virtual void ApplyCooldown(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo) const override;

	FEntriesDefinition* GetEntryManifest(const FName& EntryID) const;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Data Table")
	TSoftObjectPtr<UDataTable> Entries;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Data Table")
	FName AbilityID = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Slot")
	EEquipSlot RequiredSlot;
};
