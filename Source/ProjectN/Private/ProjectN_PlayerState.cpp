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
	DOREPLIFETIME(AProjectN_PlayerState, Level);
	DOREPLIFETIME(AProjectN_PlayerState, XP);
	DOREPLIFETIME(AProjectN_PlayerState, AttributePoints);
}

/*
 *** Main Get and Set functions
 */
UAbilitySystemComponent* AProjectN_PlayerState::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

FVector AProjectN_PlayerState::GetWeaponSocketLocation(const FGameplayTag& InputTag)
{
	return ProjectN_InventoryComponent->FindSocketLocationByTag(InputTag);
}

/*
 *
 */

void AProjectN_PlayerState::SetLevel(int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChanged.Broadcast(Level);
}

void AProjectN_PlayerState::AddToLevel(int32 NewLevel)
{
	Level += NewLevel;
	OnLevelChanged.Broadcast(Level);
}

void AProjectN_PlayerState::SetXP(int32 NewXP)
{
	XP = NewXP;
	OnXPChanged.Broadcast(XP);
}

void AProjectN_PlayerState::AddToXP(int32 NewXP)
{
	XP += NewXP;
	OnXPChanged.Broadcast(XP);
}

void AProjectN_PlayerState::SetAttributePoints(int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AProjectN_PlayerState::AddToAttributePoints(int32 NewAttributePoints)
{
	AttributePoints += NewAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AProjectN_PlayerState::OnRep_Level(int32 OldLevel)
{
	OnLevelChanged.Broadcast(Level);
}

void AProjectN_PlayerState::OnRep_XP(int32 OldXP)
{
	OnXPChanged.Broadcast(XP);
}

void AProjectN_PlayerState::OnRep_AttributePoints(int32 OldAttributePoints)
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}
