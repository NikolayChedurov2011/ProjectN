// N Chedurov All Rights Reserved


#include "ProjectN_GameplayTags.h"

namespace ProjectNGameplayTags
{
	/**** Input Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move,			"InputTag.Move")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look,			"InputTag.Look")
	
	/**** Inventory Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(InventoryTag_Equip,		"InventoryTag.Equip")
	UE_DEFINE_GAMEPLAY_TAG(InventoryTag_UnEquip,	"InventoryTag.UnEquip")
	UE_DEFINE_GAMEPLAY_TAG(InventoryTag_Drop,		"InventoryTag.Drop")
}
