// N Chedurov All Rights Reserved

#include "ProjectN_PlayerState.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Data/LevelUpDataInfo.h"
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
	DOREPLIFETIME(AProjectN_PlayerState, AttributePointsInUse);
}

/*****************************
 * Main Get and Set functions
 *****************************/
UAbilitySystemComponent* AProjectN_PlayerState::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

FVector AProjectN_PlayerState::GetWeaponSocketLocation_Implementation(const FGameplayTag& InputTag) const
{
	return ProjectN_InventoryComponent->FindSocketLocationByTag(InputTag);
}

void AProjectN_PlayerState::SetLevel(const int32 NewLevel)
{
	Level = NewLevel;
	OnLevelChanged.Broadcast(Level);
}

void AProjectN_PlayerState::AddToLevel(const int32 NewLevel)
{
	Level += NewLevel;
	OnLevelChanged.Broadcast(Level);
}

void AProjectN_PlayerState::SetXP(const int32 NewXP)
{
	XP = NewXP;
	OnXPChanged.Broadcast(XP);
}

void AProjectN_PlayerState::AddToXP(const int32 NewXP)
{
	XP += NewXP;
	OnXPChanged.Broadcast(XP);
}

void AProjectN_PlayerState::SetAttributePoints(const int32 NewAttributePoints)
{
	AttributePoints = NewAttributePoints;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

void AProjectN_PlayerState::AddToAttributePoints(const int32 AttributePointsToAdd)
{
	AttributePoints += AttributePointsToAdd;
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

int32 AProjectN_PlayerState::GetAttributePoints() const
{
	 return AttributePoints;
}

void AProjectN_PlayerState::ServerSetAttributePointsInUse_Implementation(const int32 NewAttributePointsInUse)
{
	SetAttributePointsInUse(NewAttributePointsInUse);
}

void AProjectN_PlayerState::SetAttributePointsInUse(const int32 NewAttributePointsInUse)
{
	AttributePointsInUse = NewAttributePointsInUse;
}

void AProjectN_PlayerState::AddToAttributePointsInUse(const int32 AttributePointsInUseToAdd)
{
	AttributePointsInUse += AttributePointsInUseToAdd;
}

int32 AProjectN_PlayerState::GetAttributePointsInUse() const
{
	return AttributePointsInUse;
}

void AProjectN_PlayerState::OnRep_Level(const int32 OldLevel) const
{
	OnLevelChanged.Broadcast(Level);
}

void AProjectN_PlayerState::OnRep_XP(const int32 OldXP) const
{
	OnXPChanged.Broadcast(XP);
}

void AProjectN_PlayerState::OnRep_AttributePoints(const int32 OldAttributePoints) const
{
	OnAttributePointsChanged.Broadcast(AttributePoints);
}

// Level up
int32 AProjectN_PlayerState::GetLevelByXP(const int32 InXP) const
{
	return LevelUpInfo->GetLevelByXP(InXP);
}

int32 AProjectN_PlayerState::GetXPForNextLevelUpByLevel(const int32 InLevel) const
{
	return LevelUpInfo->GetXPForNextLevelUpByLevel(InLevel);
}

int32 AProjectN_PlayerState::GetAttributePointsRewardForLevel(const int32 InLevel) const
{
	return GetLevelUpInformationContainer()[InLevel].AttributePointsReward;
}

TArray<FLevelUpInfo>& AProjectN_PlayerState::GetLevelUpInformationContainer() const
{
	return LevelUpInfo->LevelUpInformationContainer;
}
