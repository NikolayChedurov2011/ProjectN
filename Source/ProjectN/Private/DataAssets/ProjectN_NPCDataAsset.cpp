// N Chedurov All Rights Reserved


#include "DataAssets/ProjectN_NPCDataAsset.h"

FNPCDefaultData& UProjectN_NPCDataAsset::GetNPCData(ENPCRarity NPCRarity)
{
	return *NPCDataMap.Find(NPCRarity);
}
