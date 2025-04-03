// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "Interfaces/CombatInterface.h"
#include "ProjectN_CharacterBase.generated.h"

class UAttributeSet;

UCLASS(Abstract)
class PROJECTN_API AProjectN_CharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AProjectN_CharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }

protected:
	virtual void InitAbilityActorInfo();
	virtual void GiveStartupAbilitiesAndEffects();

	UPROPERTY(Transient)
	TObjectPtr<UAttributeSet> ProjectN_AttributeSet = nullptr;
	
	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent = nullptr;
	
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const;
};
