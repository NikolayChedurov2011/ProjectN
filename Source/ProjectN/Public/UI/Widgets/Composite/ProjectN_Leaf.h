// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_CompositeBase.h"
#include "ProjectN_Leaf.generated.h"


UCLASS()
class PROJECTN_API UProjectN_Leaf : public UProjectN_CompositeBase
{
	GENERATED_BODY()

public:

	virtual void ApplyFunction(FuncType Function) override;
};
