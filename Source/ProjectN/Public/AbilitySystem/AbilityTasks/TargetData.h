// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "TargetData.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FValidDataSignature, const FGameplayAbilityTargetDataHandle&, DataHandle);

UCLASS()
class PROJECTN_API UTargetData : public UAbilityTask
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "Ability | Tasks", meta=(DisplayName="TargetData", HidePin="OwningAbility", DefaultToSelf="OwningAbility", BlueprintInternalUseOnly="true"))
	static UTargetData* CreateTargetData(UGameplayAbility* OwningAbility);

	UPROPERTY(BlueprintAssignable)
	FValidDataSignature ValidData;

private:
	virtual void Activate() override;
	void SendData() const;
};
