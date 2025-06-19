#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InstancedStruct.h"
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
	ConsumableItem	UMETA(DisplayName = "ConsumableItem"),
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
};

USTRUCT(BlueprintType, Blueprintable)
struct FTagValueData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag Tag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value;
};

USTRUCT(BlueprintType, Blueprintable)
struct FConsumableItemDefinition : public FItemDefinition
{
	GENERATED_BODY()

	FConsumableItemDefinition()
	{
		ItemType = EEntryType::ConsumableItem;
	}
	
	UPROPERTY(EditDefaultsOnly)
	bool bShouldDestroyAfterUse = false;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> UseItemAbility = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTagValueData CooldownData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTagValueData CostData;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEquippableItemDefinition : public FItemDefinition
{
	GENERATED_BODY()

	FEquippableItemDefinition()
	{
		ItemType = EEntryType::Equipment;
	}
	
	UPROPERTY(EditDefaultsOnly)
	TMap<EEquipSlot, FName> SocketToAttach;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<EEquipSlot> AllowedSlots;
	
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<UGameplayEffect>> ItemPassiveEffects;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, float> ItemBonusAttributes;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectN_ItemActor_Base> ItemActorClass = nullptr;
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

	UPROPERTY(EditDefaultsOnly)
	TMap<EWeaponMode, FWeaponAbilitiesInfo> WeaponAbilitiesInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTypeTag;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<FGameplayTag, float> WeaponDamageTypes;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimSequence> TwoHandedPosture = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FAbilityDefinition : public FEntryDefinition
{
	GENERATED_BODY()

	FAbilityDefinition()
	{
		ItemType = EEntryType::Ability;
	}

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTagValueData CooldownData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTagValueData CostData;
};

USTRUCT(BlueprintType, Blueprintable)
struct FBagDefinition : public FItemDefinition
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	int32 NumSlots = 16;
};


/*******************
*   Slots Data
********************/
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
	int32 Quantity = 1;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEquipSlotData : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	FEquipSlotData(){}
	FEquipSlotData(const FName NewItemID, const EEquipSlot NewSlot) : ItemID(NewItemID) {}

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EEquipSlot EquipSlot = EEquipSlot::None;

	UPROPERTY()
	AActor* SpawnedActor = nullptr;
};


USTRUCT(BlueprintType, Blueprintable)
struct FAbilitySlotData : public FFastArraySerializerItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemID = NAME_None;
};

UENUM(BlueprintType)
enum class EMovementState : uint8
{
	None		UMETA(DisplayName = "None"),
	Run			UMETA(DisplayName = "Run"),
	Walk		UMETA(DisplayName = "Walk"),
	Crouch		UMETA(DisplayName = "Crouch"),
};

USTRUCT(BlueprintType, Blueprintable)
struct FMovementData
{
	GENERATED_BODY()
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxWalkSpeed = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float MaxAcceleration = 0.f;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingDeceleration = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingFrictionFactor = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float BrakingFriction = 0.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bUseSeparateBrakingFriction = false;
};


/*******************
*   Fragments
********************/
USTRUCT(BlueprintType, Blueprintable)
struct FFragmentData
{
	GENERATED_BODY()

	FFragmentData() {}
	FFragmentData(const FFragmentData&) = default;
	FFragmentData& operator = (const FFragmentData&) = default;
	FFragmentData(FFragmentData&) = default;
	FFragmentData& operator = (FFragmentData&) = default;
	virtual ~FFragmentData() {}

	FORCEINLINE FGameplayTag GetFragmentTag() const  { return  FragmentTag; }
	FORCEINLINE void SetFragmentTag(const FGameplayTag Tag) { FragmentTag = Tag; }

private:

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag FragmentTag = FGameplayTag();
};

USTRUCT(BlueprintType, Blueprintable)
struct FBagFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE int32 GetNumSlots() const { return NumSlots; }
	
private:

	UPROPERTY(EditDefaultsOnly)
	int32 NumSlots = 16;
};

USTRUCT(BlueprintType, Blueprintable)
struct FTypeFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE EEntryType GetEntryType() const { return EntryType; }
	
private:
	
	UPROPERTY(EditDefaultsOnly)
	EEntryType EntryType = EEntryType::None;
};

USTRUCT(BlueprintType, Blueprintable)
struct FDescriptionFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE FName GetItemName() const { return ItemName; }
	FORCEINLINE void SetItemName(const FName& Name) { ItemName = Name; }
	FORCEINLINE FString GetItemDescription() const { return ItemDescription; }
	FORCEINLINE void SetItemDescription(const FString& Description) { ItemDescription = Description; }
	
private:

	UPROPERTY(EditDefaultsOnly)
	FName ItemName = NAME_None;

	UPROPERTY(EditDefaultsOnly)
	FString ItemDescription = FString();
};

USTRUCT(BlueprintType, Blueprintable)
struct FIconFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE UTexture2D* GetIcon() const { return ItemIcon; }
	FORCEINLINE void SetIcon(const TObjectPtr<UTexture2D>& Icon) { ItemIcon = Icon; }
	
private:

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UTexture2D> ItemIcon = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FStackFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE int32 GetMaxStack() const { return MaxStack; }
	FORCEINLINE void SetMaxStack(const int32 NewMaxStack) { MaxStack = NewMaxStack; }
	FORCEINLINE int32 GetStackCount() const { return StackCount; }
	FORCEINLINE void SetStackCount(const int32 NewStackCount) { StackCount = NewStackCount; }
	
private:

	UPROPERTY(EditDefaultsOnly)
	int32 MaxStack = 1;

	UPROPERTY(EditDefaultsOnly)
	int32 StackCount = 1;
};

USTRUCT(BlueprintType, Blueprintable)
struct FConsumableFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE bool IsShouldDestroyAfterUse() const { return bShouldDestroyAfterUse; }
	FORCEINLINE void SetIsShouldDestroyAfterUse(const bool bShouldDestroy) { bShouldDestroyAfterUse = bShouldDestroy; }
	
private:

	UPROPERTY(EditDefaultsOnly)
	bool bShouldDestroyAfterUse = false;
};

USTRUCT(BlueprintType, Blueprintable)
struct FAbilityFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE TSubclassOf<UGameplayAbility> GetAbilityClass() const { return Ability; }
	FORCEINLINE void SetAbilityClass(const TSubclassOf<UGameplayAbility>& AbilityClass) { Ability = AbilityClass; }
	FORCEINLINE FGameplayTag GetCooldownTag() const { return CooldownTag; }
	FORCEINLINE void SetCooldownTag(const FGameplayTag& Tag) { CooldownTag = Tag; }
	FORCEINLINE float GetCooldownValue() const { return CooldownValue; }
	FORCEINLINE void SetCooldownValue(const float Value) { CooldownValue = Value; }
	FORCEINLINE float GetCostValue() const { return CostValue; }
	FORCEINLINE void SetCostValue(const float Value) { CostValue = Value; }

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag CooldownTag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly)
	float CooldownValue = 0.f;

	UPROPERTY(EditDefaultsOnly)
	float CostValue = 0.f;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEquippingFragment : public FFragmentData
{
	GENERATED_BODY()

	FORCEINLINE TMap<EEquipSlot, FName> GetSocketToAttach() const { return SocketToAttach; }
	FORCEINLINE void SetSocketToAttach(const TMap<EEquipSlot, FName>& NewSocketToAttach) { SocketToAttach = NewSocketToAttach; }
	FORCEINLINE TArray<EEquipSlot> GetAllowedSlots() const { return AllowedSlots; }
	FORCEINLINE void SetAllowedSlots(const TArray<EEquipSlot>& NewAllowedSlots) { AllowedSlots = NewAllowedSlots; }
	FORCEINLINE TMap<FGameplayTag, float> GetItemBonusAttributes() const { return ItemBonusAttributes; }
	FORCEINLINE void SetItemBonusAttributes(const TMap<FGameplayTag, float>& NewItemBonusAttributes) { ItemBonusAttributes = NewItemBonusAttributes; }
	FORCEINLINE TSubclassOf<AProjectN_ItemActor_Base> GetItemActorClass() const { return ItemActorClass; }
	FORCEINLINE void SetItemActorClass(const TSubclassOf<AProjectN_ItemActor_Base>& NewItemActorClass) { ItemActorClass = NewItemActorClass; }

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<EEquipSlot, FName> SocketToAttach;

	UPROPERTY(EditDefaultsOnly)
	TArray<EEquipSlot> AllowedSlots;
	
	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, float> ItemBonusAttributes;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectN_ItemActor_Base> ItemActorClass = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponFragment : public FFragmentData
{
	GENERATED_BODY()
	
	FORCEINLINE TMap<EWeaponMode, FWeaponAbilitiesInfo> GetWeaponAbilitiesInfo() const { return WeaponAbilitiesInfo; }
	FORCEINLINE void SetWeaponAbilitiesInfo(const TMap<EWeaponMode, FWeaponAbilitiesInfo>& NewWeaponAbilitiesInfo) { WeaponAbilitiesInfo = NewWeaponAbilitiesInfo; }
	FORCEINLINE FGameplayTag GetWeaponTypeTag() const { return WeaponTypeTag; }
	FORCEINLINE void SetWeaponTypeTag(const FGameplayTag& NewWeaponTypeTag) { WeaponTypeTag = NewWeaponTypeTag; }
	FORCEINLINE TMap<FGameplayTag, float> GetWeaponDamageTypes() const { return WeaponDamageTypes; }
	FORCEINLINE void SetWeaponDamageTypes(const TMap<FGameplayTag, float>& NewWeaponDamageTypes) { WeaponDamageTypes = NewWeaponDamageTypes; }
	FORCEINLINE TObjectPtr<UAnimSequence> GetTwoHandedPosture() const { return TwoHandedPosture; }
	FORCEINLINE void SetTwoHandedPosture(const TObjectPtr<UAnimSequence>& NewTwoHandedPosture) { TwoHandedPosture = NewTwoHandedPosture; }

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<EWeaponMode, FWeaponAbilitiesInfo> WeaponAbilitiesInfo;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponTypeTag;

	UPROPERTY(EditDefaultsOnly)
	TMap<FGameplayTag, float> WeaponDamageTypes;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UAnimSequence> TwoHandedPosture = nullptr;
};


/*******************************
*   Manifest and Data Table
********************************/
UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UItemManifest : public UDataAsset
{
	GENERATED_BODY()

public:

	template <typename T> requires std::derived_from<T, FFragmentData>
	const T* GetFragmentByTag(const FGameplayTag& Tag) const
	{
		for (const TInstancedStruct<FFragmentData>& Fragment : Fragments)
		{
			if (const T* FragmentPtr = Fragment.GetPtr<T>())
			{
				if (!FragmentPtr->GetFragmentTag().MatchesTagExact(Tag))
				{
					continue;
				}
				return FragmentPtr;
			}
		}
		return nullptr;
	}

protected:
	UPROPERTY(EditDefaultsOnly, meta=(ExcludeBaseStruct))
	TArray<TInstancedStruct<FFragmentData>> Fragments;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEntriesDefinition : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UItemManifest> FragmentManifest = nullptr;
};

template <typename FragmentType>
const FragmentType* GetFragment(const UItemManifest& Manifest, const FGameplayTag& Tag)
{
	return Manifest.GetFragmentByTag<FragmentType>(Tag);
}