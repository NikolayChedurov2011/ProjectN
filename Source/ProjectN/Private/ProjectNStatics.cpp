// N Chedurov All Rights Reserved


#include "ProjectNStatics.h"

const UItemStaticClass* UProjectNStatics::GetItemStaticData(TSubclassOf<UItemStaticClass> ItemDataClass)
{
	if (!IsValid(ItemDataClass))
	{
		return nullptr;
	}
	
	return ItemDataClass.GetDefaultObject();
}
