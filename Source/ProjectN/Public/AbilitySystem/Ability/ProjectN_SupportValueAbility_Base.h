// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_GameplayAbilityBase.h"
#include "ProjectN_SupportValueAbility_Base.generated.h"

UCLASS()
class PROJECTN_API UProjectN_SupportValueAbility_Base : public UProjectN_GameplayAbilityBase
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable)
	FGameplayEffectSpecHandle ApplyValue() const;
	
	void AssignValue(const FGameplayEffectSpecHandle& EffectSpecHandle) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Ability Defaults")
	TSubclassOf<UGameplayEffect> Effect = nullptr;
};
