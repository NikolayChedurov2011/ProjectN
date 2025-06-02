// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/AvatarInfoInterface.h"
#include "Interfaces/InventoryInterface.h"
#include "ProjectN_PlayerState.generated.h"

enum class EEquipSlot : uint8;
struct FOnAttributeChangeData;
struct FLevelUpInfo;
class ULevelUpDataInfo;
class UProjectN_AbilitySystemComponent;
class UAttributeSet;
class UProjectN_InventoryComponent;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGameplayValueChangedSignature, int32 /*Value*/);

UCLASS()
class PROJECTN_API AProjectN_PlayerState : public APlayerState, public IAbilitySystemInterface, public IAvatarInfoInterface, public IInventoryInterface
{
	GENERATED_BODY()

public:
	AProjectN_PlayerState();
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }

	/*************************
	*  Avatar Actor Interface
	**************************/
	virtual int32 GetCharacterLevel_Implementation() const override { return Level; }
	virtual FVector GetWeaponSocketLocation_Implementation(const EEquipSlot ItemSlot) const override;
	
	FOnGameplayValueChangedSignature OnLevelChanged;
	FOnGameplayValueChangedSignature OnXPChanged;
	FOnGameplayValueChangedSignature OnSkillTreePointsChanged;

	void SetLevel(const int32 NewLevel);
	void AddToLevel(const int32 NewLevel);
	FORCEINLINE int32 GetCharacterLevel_Internal() const { return Level; }

	void SetXP(const int32 NewXP);
	void AddToXP(const int32 NewXP);
	FORCEINLINE int32 GetXP() const { return XP; }
	
	void SetSkillTreePoints(const int32 NewAttributePoints);
	void AddToSkillTreePoints(const int32 AttributePointsToAdd);
	FORCEINLINE int32 GetSkillTreePoints() const;

	UFUNCTION(Server, Reliable)
	void ServerSetSkillTreePointsInUse(const int32 NewAttributePointsInUse);
	void SetSkillTreePointsInUse(const int32 NewAttributePointsInUse);
	void AddToSkillTreePointsInUse(const int32 AttributePointsInUseToAdd);
	FORCEINLINE int32 GetSkillTreePointsInUse() const;

	// Level up
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<ULevelUpDataInfo> LevelUpInfo;

	int32 GetLevelByXP(const int32 InXP) const;
	int32 GetXPForNextLevelUpByLevel(const int32 InLevel) const;
	int32 GetSkillTreePointsRewardForLevel(const int32 InLevel) const;
	TArray<FLevelUpInfo>& GetLevelUpInformationContainer() const;

	/*************************
	*  Inventory Interface
	**************************/
	//float GetWeaponMinDamageForSlot_Implementation(const EItemSlot ItemSlot) const override;
	TMap<FGameplayTag, float> GetWeaponDamageTypes_Implementation(const EEquipSlot ItemSlot) const override;
	AProjectN_WeaponActor* GetEquippedWeaponForSlot_Implementation(const EEquipSlot ItemSlot) const override;

protected:
	
	UPROPERTY(Transient)
	TObjectPtr<UAttributeSet> ProjectN_AttributeSet;

	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent;

	UPROPERTY(EditDefaultsOnly, Replicated)
	TObjectPtr<UProjectN_InventoryComponent> ProjectN_InventoryComponent;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_Level)
	int32 Level = 1;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_XP)
	int32 XP = 0;

	UPROPERTY(VisibleAnywhere, ReplicatedUsing=OnRep_AttributePoints)
	int32 SkillTreePoints = 0;
	UPROPERTY(VisibleAnywhere, Replicated)
	int32 SkillTreePointsInUse = 0;

	UFUNCTION()
	void OnRep_Level(const int32 OldLevel) const;
	UFUNCTION()
	void OnRep_XP(const int32 OldXP) const;
	UFUNCTION()
	void OnRep_AttributePoints(const int32 OldAttributePoints) const;
};