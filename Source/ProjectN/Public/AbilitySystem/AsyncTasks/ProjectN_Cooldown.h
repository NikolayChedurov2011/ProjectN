// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ProjectN_Cooldown.generated.h"

class UAbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FCooldownSignature, float, TimeRemaining);

UCLASS(BlueprintType, meta=(ExposedAsyncProxy="AsyncTask"))
class PROJECTN_API UProjectN_Cooldown : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FCooldownSignature CooldownStart;

	UPROPERTY(BlueprintAssignable)
	FCooldownSignature CooldownEnd;

	UFUNCTION(BlueprintCallable, meta=(BlueprintInternalUseOnly="true"))
	static UProjectN_Cooldown* WaitForCooldownChange(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

protected:

	void CooldownTagChanged(const FGameplayTag InCooldownTag, int32 NewCount);
	void OnActiveEffectAdded(UAbilitySystemComponent* TargetAbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> ASC;

	FGameplayTag CooldownTag;
};
