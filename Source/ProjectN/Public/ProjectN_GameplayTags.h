// N Chedurov All Rights Reserved

#pragma once

#include "NativeGameplayTags.h"

namespace ProjectNGameplayTags
{
	/**** Input Tags ****/
	PROJECTN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Move)
	PROJECTN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_Look)
	
	/**** Inventory Tags ****/
	PROJECTN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryTag_Equip)
	PROJECTN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryTag_UnEquip)
	PROJECTN_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InventoryTag_Drop)
}
