// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "MMC_AbilityCost.generated.h"

struct FEntriesDefinition;

UCLASS()
class PROJECTN_API UMMC_AbilityCost : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()
	
public:

	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

protected:

	FEntriesDefinition* GetEntryManifest(const FName& EntryID) const;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Ability Data Table")
	TObjectPtr<UDataTable> Entries;
};
