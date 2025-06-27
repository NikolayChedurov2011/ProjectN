#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "GameplayTagContainer.h"
#include "Net/Serialization/FastArraySerializer.h"
#include "InstancedStruct.h"
#include "ProjectNTypes.generated.h"

class UProjectN_CompositeBase;
class UProjectN_InventoryComponent;
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
	FGameplayTag Tag = FGameplayTag();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Value = 0.f;
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
	FTagValueData CooldownData = FTagValueData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTagValueData CostData = FTagValueData();
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
	FTagValueData CooldownData = FTagValueData();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FTagValueData CostData = FTagValueData();
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
struct FInventorySlotData : public FFastArraySerializerItem
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGuid BagIndex = FGuid();

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
	TArray<FInventorySlotData> Slots;
};


USTRUCT(BlueprintType)
struct FBagList : public FFastArraySerializer
{
	GENERATED_BODY()
	
	UPROPERTY()
	TArray<FInventorySlotData> Slots;

	UPROPERTY()
	TObjectPtr<UProjectN_InventoryComponent> InventoryComponent;
	
	bool NetDeltaSerialize(FNetDeltaSerializeInfo& DeltaParams)
	{
		return FFastArraySerializer::FastArrayDeltaSerialize<FInventorySlotData, FBagList>(Slots, DeltaParams, *this);
	}

	void PreReplicatedRemove(const TArrayView<int32>& RemovedIndices, int32 FinalSize);
	void PostReplicatedAdd(const TArrayView<int32>& AddedIndices, int32 FinalSize);
	void PostReplicatedChange(const TArrayView<int32>& ChangedIndices, int32 FinalSize);
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
struct FInventoryFragment : public FFragmentData
{
	GENERATED_BODY()

	virtual void Assimilate(UProjectN_CompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const UProjectN_CompositeBase* Composite) const;
};

USTRUCT(BlueprintType, Blueprintable)
struct FTextFragment : public FInventoryFragment
{
	GENERATED_BODY()

	FORCEINLINE FText GetText() const { return TextFragment; }
	FORCEINLINE void SetText(const FText& Text) { TextFragment = Text; }
	virtual void Assimilate(UProjectN_CompositeBase* Composite) const override;

private:

	UPROPERTY(EditAnywhere, Category="Default Params")
	FText TextFragment;
};

USTRUCT(BlueprintType, Blueprintable)
struct FLabeledFragment : public FInventoryFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UProjectN_CompositeBase* Composite) const override;
	
	FORCEINLINE FText GetText() const { return LabelText; }
	FORCEINLINE void SetText(const FText& NewText) { LabelText = NewText; }
	FORCEINLINE float GetValue() const { return Value; }
	FORCEINLINE void SetValue(const float NewValue) { Value = NewValue; }
	FORCEINLINE FGameplayTag GetTag() const { return ModifierTag; }

private:

	UPROPERTY(EditAnywhere, Category="Label Params")
	FText LabelText;

	UPROPERTY(EditAnywhere, Category="Label Params")
	float Value = 0.f;

	UPROPERTY(EditAnywhere, Category="Label Params")
	FGameplayTag ModifierTag = FGameplayTag();
};

USTRUCT(BlueprintType, Blueprintable)
struct FModifierFragment : public FInventoryFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UProjectN_CompositeBase* Composite) const override;

	FORCEINLINE TArray<TInstancedStruct<FLabeledFragment>> GetModifiers() const { return Modifiers; }

private:

	UPROPERTY(EditAnywhere, Category="Default Params")
	TArray<TInstancedStruct<FLabeledFragment>> Modifiers;
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
struct FConsumableFragment : public FInventoryFragment
{
	GENERATED_BODY()

	FORCEINLINE bool IsShouldDestroyAfterUse() const { return bShouldDestroyAfterUse; }
	FORCEINLINE void SetIsShouldDestroyAfterUse(const bool bShouldDestroy) { bShouldDestroyAfterUse = bShouldDestroy; }
	
private:

	UPROPERTY(EditDefaultsOnly)
	bool bShouldDestroyAfterUse = false;
};

USTRUCT(BlueprintType, Blueprintable)
struct FAbilityFragment : public FInventoryFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UProjectN_CompositeBase* Composite) const override;

	FORCEINLINE TSubclassOf<UGameplayAbility> GetAbilityClass() const { return Ability; }
	FORCEINLINE FGameplayTag GetCooldownTag() const { return Cooldown.Get<FLabeledFragment>().GetTag(); }
	FORCEINLINE float GetCooldownValue() const { return Cooldown.Get<FLabeledFragment>().GetValue(); }
	FORCEINLINE TInstancedStruct<FLabeledFragment> GetCost() const { return Cost; }
	FORCEINLINE TInstancedStruct<FLabeledFragment> GetAbilityValue() const { return Value; }

private:

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> Ability = nullptr;

	UPROPERTY(EditAnywhere, Category="Ability Params")
	TInstancedStruct<FLabeledFragment> Cooldown;

	UPROPERTY(EditAnywhere, Category="Ability Params")
	TInstancedStruct<FLabeledFragment> Cost;

	UPROPERTY(EditAnywhere, Category="Ability Params")
	TInstancedStruct<FLabeledFragment> Value;
};

USTRUCT(BlueprintType, Blueprintable)
struct FEquippingFragment : public FInventoryFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UProjectN_CompositeBase* Composite) const override;

	FORCEINLINE TMap<EEquipSlot, FName> GetSocketToAttach() const { return SocketToAttach; }
	FORCEINLINE void SetSocketToAttach(const TMap<EEquipSlot, FName>& NewSocketToAttach) { SocketToAttach = NewSocketToAttach; }
	FORCEINLINE TArray<EEquipSlot> GetAllowedSlots() const { return AllowedSlots; }
	FORCEINLINE void SetAllowedSlots(const TArray<EEquipSlot>& NewAllowedSlots) { AllowedSlots = NewAllowedSlots; }
	FORCEINLINE TInstancedStruct<FModifierFragment> GetItemBonusAttributes() const { return BonusModifiers; }
	FORCEINLINE TSubclassOf<AProjectN_ItemActor_Base> GetItemActorClass() const { return ItemActorClass; }
	FORCEINLINE void SetItemActorClass(const TSubclassOf<AProjectN_ItemActor_Base>& NewItemActorClass) { ItemActorClass = NewItemActorClass; }

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<EEquipSlot, FName> SocketToAttach;

	UPROPERTY(EditDefaultsOnly)
	TArray<EEquipSlot> AllowedSlots;

	UPROPERTY(EditAnywhere, Category="Ability Params")
	TInstancedStruct<FModifierFragment> BonusModifiers;
	
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AProjectN_ItemActor_Base> ItemActorClass = nullptr;
};

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponFragment : public FInventoryFragment
{
	GENERATED_BODY()

	virtual void Assimilate(UProjectN_CompositeBase* Composite) const override;
	
	FORCEINLINE TMap<EWeaponMode, FWeaponAbilitiesInfo> GetWeaponAbilitiesInfo() const { return WeaponAbilitiesInfo; }
	FORCEINLINE void SetWeaponAbilitiesInfo(const TMap<EWeaponMode, FWeaponAbilitiesInfo>& NewWeaponAbilitiesInfo) { WeaponAbilitiesInfo = NewWeaponAbilitiesInfo; }
	FORCEINLINE FGameplayTag GetWeaponTypeTag() const { return WeaponTypeTag; }
	FORCEINLINE void SetWeaponTypeTag(const FGameplayTag& NewWeaponTypeTag) { WeaponTypeTag = NewWeaponTypeTag; }
	FORCEINLINE TInstancedStruct<FLabeledFragment> GetWeaponDamageTypes() const { return WeaponDamageModifiers; }
	FORCEINLINE TObjectPtr<UAnimSequence> GetTwoHandedPosture() const { return TwoHandedPosture; }
	FORCEINLINE void SetTwoHandedPosture(const TObjectPtr<UAnimSequence>& NewTwoHandedPosture) { TwoHandedPosture = NewTwoHandedPosture; }

private:

	UPROPERTY(EditDefaultsOnly)
	TMap<EWeaponMode, FWeaponAbilitiesInfo> WeaponAbilitiesInfo;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag WeaponTypeTag;
	
	UPROPERTY(EditDefaultsOnly)
	TInstancedStruct<FLabeledFragment> WeaponDamageModifiers;
	
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

	template <typename T> requires std::derived_from<T, FFragmentData>
	TArray<const T*> GetAllFragmentsOfType() const
	{
		TArray<const T*> Result;
		for (const TInstancedStruct<FFragmentData>& Fragment : Fragments)
		{
			if (const T* FragmentPtr = Fragment.GetPtr<T>())
			{
				Result.Add(FragmentPtr);
			}
		}
		return Result;
	}

	void AssimilateInventoryFragments(UProjectN_CompositeBase* Composite) const;

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