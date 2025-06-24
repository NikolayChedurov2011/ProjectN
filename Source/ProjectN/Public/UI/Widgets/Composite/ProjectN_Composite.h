// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_CompositeBase.h"
#include "ProjectN_Composite.generated.h"

UCLASS()
class PROJECTN_API UProjectN_Composite : public UProjectN_CompositeBase
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	virtual void ApplyFunction(FuncType Function) override;
	virtual void Collapse() override;
	
private:

	UPROPERTY()
	TArray<TObjectPtr<UProjectN_CompositeBase>> Children;
};
