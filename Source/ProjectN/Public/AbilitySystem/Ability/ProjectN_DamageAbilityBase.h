// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_GameplayAbilityBase.h"
#include "Abilities/GameplayAbility.h"
#include "ProjectN_DamageAbilityBase.generated.h"

UCLASS()
class PROJECTN_API UProjectN_DamageAbilityBase : public UProjectN_GameplayAbilityBase
{
	GENERATED_BODY()

protected:
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	TSubclassOf<UGameplayEffect> DamageEffect = nullptr;
};
