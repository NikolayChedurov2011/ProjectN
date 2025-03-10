// N Chedurov All Rights Reserved

#include "ProjectN_PlayerState.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "Inventory/ProjectN_InventoryComponent.h"
#include "Net/UnrealNetwork.h"

AProjectN_PlayerState::AProjectN_PlayerState()
{
	NetUpdateFrequency = 100.f;
	
	ProjectN_AttributeSet = CreateDefaultSubobject<UProjectN_AttributeSet>(TEXT("ProjectN Attribute Set"));

	ProjectN_AbilitySystemComponent = CreateDefaultSubobject<UProjectN_AbilitySystemComponent>(TEXT("ProjectN Ability System Component"));
	ProjectN_AbilitySystemComponent->SetIsReplicated(true);
	ProjectN_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ProjectN_InventoryComponent = CreateDefaultSubobject<UProjectN_InventoryComponent>(TEXT("ProjectN Inventory Component"));
	ProjectN_InventoryComponent->SetIsReplicated(true);
}

void AProjectN_PlayerState::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AProjectN_PlayerState, ProjectN_InventoryComponent);
	DOREPLIFETIME(AProjectN_PlayerState, CharacterLevel);
}

/*
 *** Main Get and Set functions
 */
UAbilitySystemComponent* AProjectN_PlayerState::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}
/*
 *
 */

void AProjectN_PlayerState::OnRep_Level(int32 OldLevel)
{
	
}