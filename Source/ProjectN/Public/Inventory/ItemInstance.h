// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectNStatics.h"
#include "ProjectN/ProjectNTypes.h"
#include "ItemInstance.generated.h"

//Runtime item data. Item parameters can be change while equipped, and we want to save it
UCLASS(Blueprintable, BlueprintType)
class PROJECTN_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Init(TSubclassOf<UItemStaticClass> InItemStaticDataClass);

	UFUNCTION(BlueprintCallable, BlueprintPure)
	const UItemStaticClass* GetItemStaticClass() const { return UProjectNStatics::GetItemStaticData(ItemStaticDataClass); }
	
	UFUNCTION()
	void OnRep_IsEquipped();

	virtual void  OnEquipped();
	virtual void  OnUnequipped();

protected:

	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticClass> ItemStaticDataClass;

	UPROPERTY(ReplicatedUsing = OnRep_IsEquipped)
	bool bIsEquipped = false;
};
