// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "ProjectN_PlayerState.generated.h"

struct FOnAttributeChangeData;
class UProjectN_AbilitySystemComponent;
class UProjectN_AttributeSet;

UCLASS()
class PROJECTN_API AProjectN_PlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectN_PlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UProjectN_AttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }

protected:
	//virtual void Possess

	UPROPERTY(Transient)
	TObjectPtr<UProjectN_AttributeSet> ProjectN_AttributeSet;

	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent;

	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const;
};