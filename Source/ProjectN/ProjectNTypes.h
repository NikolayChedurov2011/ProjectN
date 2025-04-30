#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Inventory/ProjectN_ItemInstance.h"
#include "ProjectNTypes.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UBlendSpace;
class UAnimSequenceBase;
class AProjectN_ItemActor_Base;

USTRUCT(BlueprintType)
struct FAnimationData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, Category = "Animation")
	UBlendSpace* MovementBlendSpace = nullptr;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	UAnimSequenceBase* IdleAnimation = nullptr;
};

UENUM(BlueprintType)
enum class EItemState : uint8
{
	None		UMETA(DisplayName = "None"),
	Equipped	UMETA(DisplayName = "Equipped"),
	Dropped		UMETA(DisplayName = "Dropped"),
};

UENUM(BlueprintType, Blueprintable)
enum class EItemSlot : uint8
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

USTRUCT(BlueprintType, Blueprintable)
struct FWeaponAbilitiesInfo
{
	GENERATED_BODY()

	/*UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponModeTag = FGameplayTag();*/
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> MainWeaponAbility = nullptr;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> AuxiliaryWeaponAbility = nullptr;
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
	*/

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
	
protected:

	UPROPERTY(EditDefaultsOnly, Category="Sockets")
	FName TwoHandOffHandSocketName = TEXT("hand_ik_target");

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EWeaponMode, FWeaponAbilitiesInfo> WeaponAbilitiesInfo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FGameplayTag WeaponTypeTag;

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
	TSubclassOf<UGameplayAbility> AuxiliaryTwoHandWeaponAbility;*/
};