// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "DataAssets/ProjectN_AnimationDataAsset.h"
#include "Interfaces/AvatarInfoInterface.h"
#include "Interfaces/CombatInterface.h"
#include "ProjectN_CharacterBase.generated.h"

class UAttributeSet;

UCLASS(Abstract)
class PROJECTN_API AProjectN_CharacterBase : public ACharacter, public IAbilitySystemInterface, public IAvatarInfoInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AProjectN_CharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }

	/* Combat interface*/
	virtual UAnimMontage* GetHitReactMontage_Implementation() const override
	{
		return IsValid(AnimationDataAsset)? AnimationDataAsset->AnimationData.HitReactAnimation : nullptr;
	}
	virtual void Die() override;

	UFUNCTION(NetMulticast, Reliable)
	virtual void MulticastHandleDeath();
	
protected:
	virtual void InitAbilityActorInfo();
	virtual void GiveStartupAbilitiesAndEffects();
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const;

	UPROPERTY(Transient)
	TObjectPtr<UAttributeSet> ProjectN_AttributeSet = nullptr;

	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent = nullptr;

	UPROPERTY(EditDefaultsOnly, Category="Animations")
	TObjectPtr<UProjectN_AnimationDataAsset> AnimationDataAsset = nullptr;
};