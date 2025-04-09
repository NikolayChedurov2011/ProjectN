// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectN_CharacterDataAsset.generated.h"

class UGameplayEffect;
class UGameplayAbility;

USTRUCT(BlueprintType)
struct FCharacterData
{
	GENERATED_USTRUCT_BODY()

	// Initialize and handle the changes for secondary attributes in runtime. Infinite
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> InitializeAttributeDependencies;

	// Initialize primary attributes with default values "strength", "intelligence", "dexterity" etc. Instant
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> SetPrimaryAttributeDefaultValues;
	
	// Initialize main attributes "health" and "mana". Instant
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TSubclassOf<UGameplayEffect> InitHealthAndMana;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> PassiveEffects;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> PassiveAbilities;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	// Apply effect with AssignTagSetByCallerMagnitude to override primary attributes. Infinite
	//UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "GAS")
	//TSubclassOf<UGameplayEffect> OverridePrimaryAttributesEffectClass;
	
	UPROPERTY(EditAnywhere, Category = "Animation")
	class UProjectN_AnimationDataAsset* DefaultAnimationDataAsset;
};

UCLASS(BlueprintType, Blueprintable)
class PROJECTN_API UProjectN_CharacterDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly)
	FCharacterData CharacterData;
};
