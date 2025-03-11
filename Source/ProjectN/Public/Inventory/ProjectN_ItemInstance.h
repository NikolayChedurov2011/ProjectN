// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectN_ItemInstance.generated.h"

struct FGameplayAbilitySpecHandle;
struct FActiveGameplayEffectHandle;

//Runtime item data. Item parameters can be change while equipped, and we want to save it
UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UProjectN_ItemInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Init(TSubclassOf<UItemStaticClass> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UItemStaticClass* GetItemStaticClass() const;
	UFUNCTION(BlueprintCallable, BlueprintPure)
	TSubclassOf<UItemStaticClass> GetItemStaticSubClass() const { return ItemStaticDataClass; }

	virtual void  OnEquip(AActor* Owner = nullptr, const FName InSocket = FName("hand_r"));
	virtual void  OnUnEquip();
	virtual void  OnDrop();

protected:

	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticClass> ItemStaticDataClass;

	UPROPERTY(ReplicatedUsing = OnRep_IsEquipped)
	bool bIsEquipped = false;

	UPROPERTY(Replicated)
	TObjectPtr<AProjectN_ItemActor_Base> ItemActor = nullptr;
	UPROPERTY(Replicated)
	TObjectPtr<ACharacter> OwnerCharacter = nullptr;

	UFUNCTION()
	void OnRep_IsEquipped();

	void ApplyItemAbilityAndEffects(const AActor* InActor);
	void RemoveItemAbilityAndEffects(const ACharacter* InCharacter);

private:

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GameplayAbilitySpecHandles;
	
	UPROPERTY()
	TArray<FActiveGameplayEffectHandle> ActiveGameplayEffectHandles;
};
