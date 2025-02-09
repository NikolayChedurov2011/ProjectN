// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN/ProjectNTypes.h"
#include "UObject/NoExportTypes.h"
#include "ItemInstance.generated.h"

//Runtime item data. Item parameters can be change while equipped, and we want to save it
UCLASS(Abstract)
class PROJECTN_API UItemInstance : public UObject
{
	GENERATED_BODY()

public:

	virtual bool IsSupportedForNetworking() const override { return true; }
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void Init(TSubclassOf<UItemStaticClass> InItemStaticDataClass);
	
	UPROPERTY(Replicated)
	TSubclassOf<UItemStaticClass> ItemStaticDataClass;

	UPROPERTY(ReplicatedUsing = OnRep_IsEquipped)
	bool bIsEquipped = false;
	
	UFUNCTION()
	void OnRep_IsEquipped();

	virtual void  OnEquipped();
	virtual void  OnUnequipped();
};
