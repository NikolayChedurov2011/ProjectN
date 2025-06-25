// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_Leaf.h"
#include "ProjectN_Leaf_Labeled.generated.h"


class UTextBlock;

UCLASS()
class PROJECTN_API UProjectN_Leaf_Labeled : public UProjectN_Leaf
{
	GENERATED_BODY()

public:
	void SetLabelText(const FText& NewLabelText, bool bCollapse) const;
	void SetValueText(const FText& NewValueText, bool bCollapse) const;

private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Label;

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Value;
	
};
