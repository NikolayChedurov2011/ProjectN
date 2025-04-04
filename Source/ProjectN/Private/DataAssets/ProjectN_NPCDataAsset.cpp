// N Chedurov All Rights Reserved


#include "DataAssets/ProjectN_NPCDataAsset.h"

FNPCDefaultData UProjectN_NPCDataAsset::GetNPCData(const ENPCRarity NPCRarity)
{
	return *NPCDataMap.Find(NPCRarity);
}
