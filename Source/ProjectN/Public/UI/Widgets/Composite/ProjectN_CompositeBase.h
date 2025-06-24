// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "ProjectN_CompositeBase.generated.h"

UCLASS()
class PROJECTN_API UProjectN_CompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:

	FGameplayTag GetFragmentTag() const { return FragmentTag; }
	void SetFragmentTag(const FGameplayTag& NewFragmentTag) { FragmentTag = NewFragmentTag; }
	virtual void Collapse();
	void Expand();

	using FuncType = TFunction<void(UProjectN_CompositeBase*)>;
	virtual void ApplyFunction(FuncType Function) {}
	
private:

	UPROPERTY(EditAnywhere, Category="Default Params")
	FGameplayTag FragmentTag = FGameplayTag();
};
