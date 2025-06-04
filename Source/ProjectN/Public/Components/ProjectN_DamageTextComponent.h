// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "ProjectN_DamageTextComponent.generated.h"

UCLASS()
class PROJECTN_API UProjectN_DamageTextComponent : public UWidgetComponent
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void SetDamageText(const float Damage, const bool bBlocked, const bool bCriticalHit, const bool bEvaded);
};