// N Chedurov All Rights Reserved


#include "ProjectN_Statics.h"

const UItemStaticClass* UProjectN_Statics::GetItemStaticData(const TSubclassOf<UItemStaticClass> ItemDataClass)
{
	if (!IsValid(ItemDataClass))
	{
		return nullptr;
	}
	
	return ItemDataClass.GetDefaultObject();
}
