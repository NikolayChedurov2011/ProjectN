// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
//#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_ItemInstance.generated.h"

struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;
class UItemStaticClass;
class AProjectN_ItemActor_Base;

//Runtime item data. Item parameters can be change while equipped, and we want to save it
UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_ItemInstance : public UObject
{
	GENERATED_BODY()

public:

//	UProjectN_ItemInstance(){}
//	UProjectN_ItemInstance(const int32 ItemStack) : ItemStack(ItemStack) {}

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Init(const TSubclassOf<UItemStaticClass> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UItemStaticClass* GetItemStaticClass() const;
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<UItemStaticClass> GetItemStaticSubClass() const { return ItemStaticDataClass; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 GetItemStack() const { return ItemStack ;}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE int32 UseItem();

	FORCEINLINE void AddItemStack(const int32 StackToAdd);
	FORCEINLINE void InitItemStack(const int32 InitiateStack);

protected:

	UPROPERTY(EditAnywhere, Replicated)
	TSubclassOf<UItemStaticClass> ItemStaticDataClass;

	UPROPERTY(EditAnywhere, Replicated)
	int32 ItemStack = 1;
};

UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_EquippableItemInstance : public UProjectN_ItemInstance
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void  OnEquip(AActor* Owner = nullptr, const FName InSocket = FName("hand_r")/*, const FGameplayTag& InputTag = FGameplayTag()*/);
	virtual void  OnUnEquip();
	virtual void  OnDrop();

	FORCEINLINE AProjectN_ItemActor_Base* GetSpawnedItemActor() const { return ItemActor; }
	FORCEINLINE FVector GetItemSocketLocationForProjectile() const;
	FORCEINLINE AProjectN_ItemActor_Base* GetItemActor() const { return ItemActor; }
	
protected:

	UPROPERTY(ReplicatedUsing = OnRep_IsEquipped)
	bool bIsEquipped = false;

	UPROPERTY(Replicated)
	TObjectPtr<AProjectN_ItemActor_Base> ItemActor = nullptr;
	UPROPERTY(Replicated)
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UFUNCTION()
	void OnRep_IsEquipped();

	//void ApplyItemAbilityAndEffects(const AActor* InActor, const FGameplayTag& InputTag = FGameplayTag());
	void AddItemPassiveEffects(const AActor* InActor, const FGameplayTag& InputTag = FGameplayTag());
	//void AddMainWeaponAbility(const AActor* InActor, const FGameplayTag& InputTag = FGameplayTag(), const FGameplayTag& WeaponModeTag = FGameplayTag());
	//void AddAuxiliaryWeaponAbility(const AActor* InActor, const FGameplayTag& InputTag = FGameplayTag(), const FGameplayTag& WeaponModeTag = FGameplayTag());
	//void AddDualWeaponAbilities(const AActor* InActor, const FGameplayTag& MainInputTag = FGameplayTag(), const FGameplayTag& AuxiliaryInputTag = FGameplayTag());
	//void AddWeaponAbilitiesByWeaponModeTag(const AActor* InActor, const FGameplayTag& MainInputTag = FGameplayTag(), const FGameplayTag& AuxiliaryInputTag = FGameplayTag(), const FGameplayTag& WeaponModeTag = FGameplayTag());
	void RemoveItemAbilityAndEffects(const ACharacter* InCharacter);
	
private:

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GameplayAbilitySpecHandles;

	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
};