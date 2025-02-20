// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ProjectN_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTN_API UProjectN_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UFUNCTION(Blueprintable, meta=(BlueprintThreadSafe))
	class UBlendSpace* GetLocomotionBlendSpace() const;
	
	UFUNCTION(Blueprintable, meta=(BlueprintThreadSafe))
	class UAnimSequenceBase* GetIdleAnimationAsset() const;

	UPROPERTY(EditAnywhere, Category = "Animation")
	class UProjectN_AnimationDataAsset* DefaultAnimationDataAsset;
};
