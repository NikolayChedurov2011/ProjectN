// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "MVVM_SaveSlot.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSetSaveSlotStateSignature, int32, Index);

UCLASS()
class PROJECTN_API UMVVM_SaveSlot : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintAssignable)
	FSetSaveSlotStateSignature SetSaveSlotState;

	void InitializeSlot();
};
