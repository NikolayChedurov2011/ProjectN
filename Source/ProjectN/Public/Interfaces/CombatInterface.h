// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

struct FGameplayTag;

UINTERFACE(MinimalAPI)
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECTN_API ICombatInterface
{
	GENERATED_BODY()

public:

	virtual int32 GetCharacterLevel();
	virtual FVector GetWeaponSocketLocation(const FGameplayTag& InputTag);
};
