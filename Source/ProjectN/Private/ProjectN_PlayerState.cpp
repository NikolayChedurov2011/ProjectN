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
	DOREPLIFETIME(AProjectN_PlayerState, SkillTreePoints);
	DOREPLIFETIME(AProjectN_PlayerState, SkillTreePointsInUse);
}

/*****************************
 * Main Get and Set functions
 *****************************/
UAbilitySystemComponent* AProjectN_PlayerState::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

FVector AProjectN_PlayerState::GetWeaponSocketLocation_Implementation(const EEquipSlot ItemSlot) const
{
	return ProjectN_InventoryComponent->FindWeaponSocketLocationForProjectileBySlot(ItemSlot);
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

void AProjectN_PlayerState::SetSkillTreePoints(const int32 NewAttributePoints)
{
	SkillTreePoints = NewAttributePoints;
	OnSkillTreePointsChanged.Broadcast(SkillTreePoints);
}

void AProjectN_PlayerState::AddToSkillTreePoints(const int32 AttributePointsToAdd)
{
	SkillTreePoints += AttributePointsToAdd;
	OnSkillTreePointsChanged.Broadcast(SkillTreePoints);
}

int32 AProjectN_PlayerState::GetSkillTreePoints() const
{
	 return SkillTreePoints;
}

void AProjectN_PlayerState::ServerSetSkillTreePointsInUse_Implementation(const int32 NewAttributePointsInUse)
{
	SetSkillTreePointsInUse(NewAttributePointsInUse);
}

void AProjectN_PlayerState::SetSkillTreePointsInUse(const int32 NewAttributePointsInUse)
{
	SkillTreePointsInUse = NewAttributePointsInUse;
}

void AProjectN_PlayerState::AddToSkillTreePointsInUse(const int32 AttributePointsInUseToAdd)
{
	SkillTreePointsInUse += AttributePointsInUseToAdd;
}

int32 AProjectN_PlayerState::GetSkillTreePointsInUse() const
{
	return SkillTreePointsInUse;
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
	OnSkillTreePointsChanged.Broadcast(SkillTreePoints);
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

int32 AProjectN_PlayerState::GetSkillTreePointsRewardForLevel(const int32 InLevel) const
{
	return GetLevelUpInformationContainer()[InLevel].SkillTreePointsReward;
}

TArray<FLevelUpInfo>& AProjectN_PlayerState::GetLevelUpInformationContainer() const
{
	return LevelUpInfo->LevelUpInformationContainer;
}

/*************************
*  Inventory Interface
**************************/
TMap<FGameplayTag, float> AProjectN_PlayerState::GetWeaponDamageTypes_Implementation(const EEquipSlot ItemSlot) const
{
	if (ItemSlot == EEquipSlot::None)
	{
		TMap<FGameplayTag, float> EmptyMap;
		return EmptyMap;
	}
	
	return ProjectN_InventoryComponent->GetWeaponDamageTypesForSlot(ItemSlot);
}

/*
float AProjectN_PlayerState::GetWeaponMaxDamageForSlot_Implementation(const EItemSlot ItemSlot) const
{
	if (ItemSlot == EItemSlot::None)
	{
		return 0.0f;
	}
	
	return ProjectN_InventoryComponent->GetWeaponMaxDamageForSlot(ItemSlot);
}
*/

AProjectN_WeaponActor* AProjectN_PlayerState::GetEquippedWeaponForSlot_Implementation(const EEquipSlot ItemSlot) const
{
	if (ItemSlot == EEquipSlot::None)
	{
		return nullptr;
	}
	
	return ProjectN_InventoryComponent->GetEquippedWeaponActorBySlot(ItemSlot);
}
