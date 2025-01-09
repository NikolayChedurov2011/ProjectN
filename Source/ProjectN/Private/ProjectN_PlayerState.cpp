// N Chedurov All Rights Reserved

#include "ProjectN_PlayerState.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"

AProjectN_PlayerState::AProjectN_PlayerState()
{
	NetUpdateFrequency = 100.f;
	
	ProjectN_AttributeSet = CreateDefaultSubobject<UProjectN_AttributeSet>(TEXT("ProjectN Attribute Set"));

	ProjectN_AbilitySystemComponent = CreateDefaultSubobject<UProjectN_AbilitySystemComponent>(TEXT("ProjectN Ability System Component"));
	ProjectN_AbilitySystemComponent->SetIsReplicated(true);
	ProjectN_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

/*
 *** Main Get and Set functions
 */
UAbilitySystemComponent* AProjectN_PlayerState::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}
