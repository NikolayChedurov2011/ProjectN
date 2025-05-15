// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "ProjectN_AbilitySystemGlobals.generated.h"

UCLASS()
class PROJECTN_API UProjectN_AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;

public:

protected:
};
