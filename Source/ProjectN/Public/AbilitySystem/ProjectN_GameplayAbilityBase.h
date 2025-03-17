// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectN_GameplayAbilityBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UProjectN_GameplayAbilityBase : public UGameplayAbility
{
	GENERATED_BODY()

public:

	FORCEINLINE const FGameplayTag& GetStartupTag() const { return StartupTag; }

protected:
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	FGameplayTag StartupTag;
};
