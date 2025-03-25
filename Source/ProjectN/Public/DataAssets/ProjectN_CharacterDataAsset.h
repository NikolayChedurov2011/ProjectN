// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_CharacterDataAsset.generated.h"

UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_CharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FCharacterData CharacterData;
};
