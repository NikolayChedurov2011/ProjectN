// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectNStatics.generated.h"

UCLASS()
class PROJECTN_API UProjectNStatics : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static const UItemStaticClass* GetItemStaticData(TSubclassOf<UItemStaticClass> ItemDataClass);
};
