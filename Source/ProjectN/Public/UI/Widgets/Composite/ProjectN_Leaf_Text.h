// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Composite/ProjectN_Leaf.h"
#include "ProjectN_Leaf_Text.generated.h"

class UTextBlock;

UCLASS()
class PROJECTN_API UProjectN_Leaf_Text : public UProjectN_Leaf
{
	GENERATED_BODY()

public:
	void SetText(const FText& NewText) const;
	
private:
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> Text;
};