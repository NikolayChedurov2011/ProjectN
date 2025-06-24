// N Chedurov All Rights Reserved


#include "UI/Widgets/Composite/ProjectN_Composite.h"

#include "Blueprint/WidgetTree.h"

void UProjectN_Composite::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	WidgetTree->ForEachWidget([this](UWidget* Widget)
	{
		UProjectN_CompositeBase* Composite = Cast<UProjectN_CompositeBase>(Widget);

		if (!IsValid(Composite))
		{
			return;
		}

		Children.Add(Composite);
		Composite->Collapse();
	});
}

void UProjectN_Composite::ApplyFunction(FuncType Function)
{
	for (TObjectPtr<UProjectN_CompositeBase>& Child : Children)
	{
		Child->ApplyFunction(Function);
	}
}

void UProjectN_Composite::Collapse()
{
	for (TObjectPtr<UProjectN_CompositeBase>& Child : Children)
	{
		Child->Collapse();
	}
}
