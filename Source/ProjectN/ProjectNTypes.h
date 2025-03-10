#pragma once

#include "CoreMinimal.h"
#include "ProjectNTypes.generated.h"

class UGameplayEffect;
class UGameplayAbility;
class UBlendSpace;
class UAnimSequenceBase;
class AProjectN_ItemActor_Base;

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	// Initialize primary attributes. Instant
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> PrimaryAttributes;

	// Initialize and handle the changes for secondary attributes in runtime. Infinite
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> SecondaryAttributes;

	// Initialize main attributes "health", "mana", "stamina" etc. Instant
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> InitializeMainAttributes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> Effects;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> Abilities;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	class UProjectN_AnimationDataAsset* DefaultAnimationDataAsset;
};

USTRUCT(BlueprintType)
struct FAnimationData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
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

UENUM(BlueprintType)
enum class EItemSlot : uint8
{
	None		UMETA(DisplayName = "None"),
	Head		UMETA(DisplayName = "Head"),
	Body		UMETA(DisplayName = "Body"),
	Legs		UMETA(DisplayName = "Legs"),
	Feet		UMETA(DisplayName = "Feet"),
	LeftArm		UMETA(DisplayName = "LeftArm"),
	RightArm	UMETA(DisplayName = "RightArm"),
};

UCLASS(BlueprintType, Blueprintable)
class UItemStaticClass : public UObject
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetItemName() const { return ItemName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE FName GetSocketToAttach() const { return SocketToAttach; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE bool CanBeEquipped() const { return bCanBeEquipped; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE EItemSlot GetItemSlot() const { return ItemSlot; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TSubclassOf<AProjectN_ItemActor_Base> GetItemActorClass() const { return ItemActorClass; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TArray<TSubclassOf<UGameplayAbility>> GetItemAbilities() const { return ItemAbilities; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FORCEINLINE TArray<TSubclassOf<UGameplayEffect>> GetItemEffects() const { return ItemEffects; }

protected:

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName ItemName = NAME_None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FName SocketToAttach = NAME_None;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	EItemSlot ItemSlot = EItemSlot::None;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<AProjectN_ItemActor_Base> ItemActorClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FAnimationData AnimationData;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	bool bCanBeEquipped = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayAbility>> ItemAbilities;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<TSubclassOf<UGameplayEffect>> ItemEffects;
};