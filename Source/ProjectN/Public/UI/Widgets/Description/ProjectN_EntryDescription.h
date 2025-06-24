// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "UI/Widgets/Composite/ProjectN_Composite.h"
#include "ProjectN_EntryDescription.generated.h"

class USizeBox;

UCLASS()
class PROJECTN_API UProjectN_EntryDescription : public UProjectN_Composite
{
	GENERATED_BODY()

public:

	FVector2D GetBoxSize() const;

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
