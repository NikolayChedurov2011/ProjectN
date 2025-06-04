#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "ProjectNTypes.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UBlendSpace;
class UAnimSequenceBase;
class AProjectN_ItemActor_Base;

USTRUCT(BlueprintType)
struct FProjectNGameplayEffectContext : public FGameplayEffectContext
{
	GENERATED_BODY()

public:
	virtual UScriptStruct* GetScriptStruct() const override
	{
		return StaticStruct();
	}
	
	/** Creates a copy of this context, used to duplicate for later modifications */
	virtual FProjectNGameplayEffectContext* Duplicate() const override
	{
		FProjectNGameplayEffectContext* NewContext = new FProjectNGameplayEffectContext();
		*NewContext = *this;
		if (GetHitResult())
		{
			// Does a deep copy of the hit result
			NewContext->AddHitResult(*GetHitResult(), true);
		}
		return NewContext;
	}
	
	virtual bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess) override
	{
		uint32 RepBits = 0;
		if (Ar.IsSaving())
		{
			if (bReplicateInstigator && Instigator.IsValid())
			{
				RepBits |= 1 << 0;
			}
			if (bReplicateEffectCauser && EffectCauser.IsValid() )
			{
				RepBits |= 1 << 1;
			}
			if (AbilityCDO.IsValid())
			{
				RepBits |= 1 << 2;
			}
			if (bReplicateSourceObject && SourceObject.IsValid())
			{
				RepBits |= 1 << 3;
			}
			if (Actors.Num() > 0)
			{
				RepBits |= 1 << 4;
			}
			if (HitResult.IsValid())
			{
				RepBits |= 1 << 5;
			}
			if (bHasWorldOrigin)
			{
				RepBits |= 1 << 6;
			}
			if (bCriticalHit)
			{
				RepBits |= (1 << 7);
			}
			if (bBlock)
			{
				RepBits |= (1 << 8);
			}
			if (bEvade)
			{
				RepBits |= (1 << 9);
			}
		}

		Ar.SerializeBits(&RepBits, 9);

		if (RepBits & (1 << 0))
		{
			Ar << Instigator;
		}
		if (RepBits & (1 << 1))
		{
			Ar << EffectCauser;
		}
		if (RepBits & (1 << 2))
		{
			Ar << AbilityCDO;
		}
		if (RepBits & (1 << 3))
		{
			Ar << SourceObject;
		}
		if (RepBits & (1 << 4))
		{
			SafeNetSerializeTArray_Default<31>(Ar, Actors);
		}
		if (RepBits & (1 << 5))
		{
			if (Ar.IsLoading())
			{
				if (!HitResult.IsValid())
				{
					HitResult = TSharedPtr<FHitResult>(new FHitResult());
				}
			}
			HitResult->NetSerialize(Ar, Map, bOutSuccess);
		}
		if (RepBits & (1 << 6))
		{
			Ar << WorldOrigin;
			bHasWorldOrigin = true;
		}
		else
		{
			bHasWorldOrigin = false;
		}
		if (RepBits & (1 << 7))
		{
			Ar << bCriticalHit;
		}
		if (RepBits & (1 << 8))
		{
			Ar << bBlock;
		}
		if (RepBits & (1 << 9))
		{
			Ar << bEvade;
		}
		
		if (Ar.IsLoading())
		{
			AddInstigator(Instigator.Get(), EffectCauser.Get()); // Just to initialize InstigatorAbilitySystemComponent
		}	
	
		bOutSuccess = true;
		return true;
	}

	FORCEINLINE bool IsCriticalHit() const { return bCriticalHit; }
	FORCEINLINE void SetIsCriticalHit(const bool bCritical) { bCriticalHit = bCritical; }
	FORCEINLINE bool IsBlocked() const { return bBlock; }
	FORCEINLINE void SetIsBlock(const bool bBlocked) { bBlock = bBlocked; }
	FORCEINLINE void SetIsEvaded(const bool bEvaded) { bEvade = bEvaded; }
	FORCEINLINE bool IsEvaded() const { return bEvade; }
	
protected:

	UPROPERTY()
	bool bCriticalHit = false;
	
	UPROPERTY()
	bool bBlock = false;

	UPROPERTY()
	bool bEvade = false;
};

template<>
struct TStructOpsTypeTraits< FProjectNGameplayEffectContext > : public TStructOpsTypeTraitsBase2< FGameplayEffectContext >
{
	enum
	{
		WithNetSerializer = true,
		WithCopy = true		// Necessary so that TSharedPtr<FHitResult> Data is copied around
	};
};

USTRUCT(BlueprintType)
struct FAnimationData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Animation")
	UBlendSpace* MovementBlendSpace = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequenceBase* IdleAnimation = nullptr;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimMontage* HitReactAnimation = nullptr;
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None		UMETA(DisplayName = "None"),
	Equipped	UMETA(DisplayName = "Equipped"),
	Dropped		UMETA(DisplayName = "Dropped"),
};

UENUM(BlueprintType, Blueprintable)
enum class EEquipSlot : uint8
{
	None			UMETA(DisplayName = "None"),
	Head			UMETA(DisplayName = "Head"),
	Body			UMETA(DisplayName = "Body"),
	Legs			UMETA(DisplayName = "Legs"),
	Feet			UMETA(DisplayName = "Feet"),
	AuxiliaryArm	UMETA(DisplayName = "AuxiliaryArm"),
	MainArm			UMETA(DisplayName = "MainArm"),
	TwoHand			UMETA(DisplayName = "TwoHand"),
};

UENUM(BlueprintType, Blueprintable)
enum class EWeaponMode : uint8
{
	None			UMETA(DisplayName = "None"),
	Single			UMETA(DisplayName = "Single"),
	TwoHand			UMETA(DisplayName = "TwoHand"),
	Dual			UMETA(DisplayName = "Dual"),
};

UENUM(BlueprintType, Blueprintable)
enum class EWeaponType : uint8
{
	None			UMETA(DisplayName = "None"),
	Dagger			UMETA(DisplayName = "Dagger"),
	ShortSword		UMETA(DisplayName = "ShortSword"),
	Bow				UMETA(DisplayName = "Bow"),
	MagicStaff		UMETA(DisplayName = "MagicStaff"),
};

/*
UCLASS(BlueprintType, Blueprintable)
class UItemStaticClass : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE UTexture2D* GetItemIcon() const { return ItemIcon; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetItemName() const { return ItemName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FString GetItemDescription() const { return ItemDescription; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool CanBeEquipped() const { return bCanBeEquipped; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool CanStack() const { return bCanStack; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool CanBeUsed() const { return bCanBeUsed; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool ShouldDestroyAfterUse() const { return bShouldDestroyAfterUse; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<AProjectN_ItemActor_Base> GetItemActorClass() const { return ItemActorClass; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<UGameplayAbility> GetUseItemAbility() const { return UseItemAbility; }
	
	virtual UProjectN_ItemInstance* CreateInstance(UObject* Outer) const
	{
		return NewObject<UProjectN_ItemInstance>(Outer, ItemInstanceClass);
	}
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Instance")
	TSubclassOf<UProjectN_ItemInstance> ItemInstanceClass = UProjectN_ItemInstance::StaticClass();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription = FString();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeEquipped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeUsed = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldDestroyAfterUse = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanStack = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectN_ItemActor_Base> ItemActorClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> UseItemAbility;
};

UCLASS(BlueprintType, Blueprintable)
class UEquippableItemStaticClass : public UItemStaticClass
{
	GENERATED_BODY()

public:

	UEquippableItemStaticClass()
	{
		bCanBeEquipped = true;
		ItemInstanceClass = UProjectN_EquippableItemInstance::StaticClass();
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetSocketsToAttach(const EItemSlot InSlot) const
	{
		for (const TTuple<EItemSlot, FName>& SocketMap : SocketToAttach)
		{
			if (SocketMap.Key == InSlot)
			{
				return SocketMap.Value;
			}
		}

		return NAME_None;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool IsAllowedSlot(const EItemSlot InSlot) const
	{
		for (const EItemSlot& Slot : AllowedSlots)
		{
			if (Slot == InSlot)
			{
				return true;
			}
		}

		return false;
	}
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TArray<TSubclassOf<UGameplayEffect>> GetItemPassiveEffects() const { return ItemPassiveEffects; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TMap<FGameplayTag, float> GetItemBonusAttributes() const { return ItemBonusAttributes; }

protected:
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EItemSlot, FName> SocketToAttach;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EItemSlot> AllowedSlots;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> ItemPassiveEffects;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, float> ItemBonusAttributes;
};



UCLASS(BlueprintType, Blueprintable)
class UWeaponItemStaticClass : public UEquippableItemStaticClass
{
	GENERATED_BODY()

public:

	/*
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<UGameplayAbility> GetMainWeaponAbility() const { return MainWeaponAbility; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<UGameplayAbility> GetAuxiliaryWeaponAbility() const { return AuxiliaryWeaponAbility; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<UGameplayAbility> GetMainDualWeaponAbility() const { return MainDualWeaponAbility; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<UGameplayAbility> GetAuxiliaryDualWeaponAbility() const { return AuxiliaryDualWeaponAbility; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<UGameplayAbility> GetMainTwoHandWeaponAbility() const { return MainTwoHandWeaponAbility; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<UGameplayAbility> GetAuxiliaryTwoHandWeaponAbility() const { return AuxiliaryTwoHandWeaponAbility; }
	#1#

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetTwoHandOffHandSocketName() const { return TwoHandOffHandSocketName; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE bool GetWeaponAbilitiesInfo(const EWeaponMode WeaponMode, FWeaponAbilitiesInfo& WeaponAbilitiesInfoOut)
	{
		const FWeaponAbilitiesInfo* Found = WeaponAbilitiesInfo.Find(WeaponMode);

		if (Found)
		{
			WeaponAbilitiesInfoOut = *Found;
			return true;
		}

		return false;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FGameplayTag GetWeaponTypeTag() const { return WeaponTypeTag; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TMap<FGameplayTag, float> GetWeaponDamageTypes() const { return WeaponDamageTypes; }
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Sockets")
	FName TwoHandOffHandSocketName = TEXT("hand_ik_target");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EWeaponMode, FWeaponAbilitiesInfo> WeaponAbilitiesInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, float> WeaponDamageTypes;

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> MainWeaponAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AuxiliaryWeaponAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> MainDualWeaponAbility;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AuxiliaryDualWeaponAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> MainTwoHandWeaponAbility;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AuxiliaryTwoHandWeaponAbility;#1#
};
*/


UENUM(BlueprintType, Blueprintable)
enum class EEntryType : uint8
{
	None			UMETA(DisplayName = "None"),
	Item			UMETA(DisplayName = "Item"),
	Equipment		UMETA(DisplayName = "Equipment"),
	Weapon			UMETA(DisplayName = "Weapon"),
	Ability			UMETA(DisplayName = "Ability"),
	Bag				UMETA(DisplayName = "Bag"),
};

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponAbilitiesInfo
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> MainWeaponAbility = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AuxiliaryWeaponAbility = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEntryDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FString ItemDescription = FString();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEntryType ItemType = EEntryType::Item;
};

USTRUCT(BlueprintType, Blueprintable)
struct FItemDefinition : public FEntryDefinition
{
	GENERATED_BODY()

	FItemDefinition()
	{
		ItemType = EEntryType::Item;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 MaxStack = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bShouldDestroyAfterUse = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> UseItemAbility;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEquippableItemDefinition : public FItemDefinition
{
	GENERATED_BODY()

	FEquippableItemDefinition()
	{
		ItemType = EEntryType::Equipment;
	}
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EEquipSlot, FName> SocketToAttach;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EEquipSlot> AllowedSlots;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> ItemPassiveEffects;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, float> ItemBonusAttributes;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectN_ItemActor_Base> ItemActorClass;
};

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponItemDefinition : public FEquippableItemDefinition
{
	GENERATED_BODY()
	
	FWeaponItemDefinition()
	{
		ItemType = EEntryType::Weapon;
	}

	UPROPERTY(EditDefaultsOnly, Category="Sockets")
	FName TwoHandOffHandSocketName = TEXT("hand_ik_target");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EWeaponMode, FWeaponAbilitiesInfo> WeaponAbilitiesInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, float> WeaponDamageTypes;
};


USTRUCT(BlueprintType, Blueprintable)
struct FAbilityDefinition : public FEntryDefinition
{
	GENERATED_BODY()

	FAbilityDefinition()
	{
		ItemType = EEntryType::Ability;
	}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FBagDefinition : public FItemDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumSlots = 16;
};

USTRUCT(BlueprintType, Blueprintable)
struct FInventorySlotData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagIndex = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 SlotIndex = INDEX_NONE;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEntryType ItemType = EEntryType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 Quantity = 1;
};

USTRUCT(BlueprintType)
struct FBagData : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 BagIndex = INDEX_NONE;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName BagItemID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FInventorySlotData> Slots;
};


USTRUCT(BlueprintType)
struct FBagList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FBagData> Bags;
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FBagData, FBagList>(Bags, DeltaParams, *this);
	}
};

template<>
struct TStructOpsTypeTraits<FBagList> : public TStructOpsTypeTraitsBase2<FBagList>
{
	enum { WithNetDeltaSerializer = true };
};

USTRUCT(BlueprintType, Blueprintable)
struct FActionSlotData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 ActionSlotIndex = INDEX_NONE;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEntryType EntryType = EEntryType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEquipSlotData : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	FEquipSlotData(){}
	FEquipSlotData(const FName NewItemID, const EEntryType NewItemType, const EEquipSlot NewSlot) : ItemID(NewItemID), ItemType(NewItemType), EquipSlot(NewSlot) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEntryType ItemType = EEntryType::None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipSlot EquipSlot = EEquipSlot::None;

	UPROPERTY()
	AActor* SpawnedActor = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;
};


USTRUCT(BlueprintType, Blueprintable)
struct FAbilitySlotData : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEntryType ItemType = EEntryType::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* ItemIcon = nullptr;
};