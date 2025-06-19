// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ProjectN_EntryDescription.generated.h"

class USizeBox;

UCLASS()
class PROJECTN_API UProjectN_EntryDescription : public UUserWidget
{
	GENERATED_BODY()

public:

	FVector2D GetBoxSize() const;

private:

	UPROPERTY(meta=(BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
