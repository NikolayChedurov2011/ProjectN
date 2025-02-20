// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_AnimationDataAsset.generated.h"

/**
 * 
 */
UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_AnimationDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FAnimationData AnimationData;
};
