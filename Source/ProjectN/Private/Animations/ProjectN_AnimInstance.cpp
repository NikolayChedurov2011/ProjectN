// N Chedurov All Rights Reserved


#include "Animations/ProjectN_AnimInstance.h"

#include "ProjectNCharacter_Base.h"
#include "DataAssets/ProjectN_AnimationDataAsset.h"

class UBlendSpace* UProjectN_AnimInstance::GetLocomotionBlendSpace() const
{
	if (AProjectNCharacter_Base* OwningCharacter = Cast<AProjectNCharacter_Base>(GetOwningActor()))
	{
		const FCharacterData CharacterData  = OwningCharacter->GetCharacterData();

		if (CharacterData.DefaultAnimationDataAsset)
		{
			return	CharacterData.DefaultAnimationDataAsset->AnimationData.MovementBlendSpace;
		}
	}

	return DefaultAnimationDataAsset ? DefaultAnimationDataAsset->AnimationData.MovementBlendSpace : nullptr;
	
}

class UAnimSequenceBase* UProjectN_AnimInstance::GetIdleAnimationAsset() const
{
	if (AProjectNCharacter_Base* OwningCharacter = Cast<AProjectNCharacter_Base>(GetOwningActor()))
	{
		const FCharacterData CharacterData  = OwningCharacter->GetCharacterData();

		if (CharacterData.DefaultAnimationDataAsset)
		{
			return	CharacterData.DefaultAnimationDataAsset->AnimationData.IdleAnimation;
		}
	}

	return DefaultAnimationDataAsset ? DefaultAnimationDataAsset->AnimationData.IdleAnimation : nullptr;
}
