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

	/**** Message Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Message,					"Message")
	UE_DEFINE_GAMEPLAY_TAG(Message_Potion,			"Message.Potion")

	/**** Attributes Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Strength,						"Attribute.Primary.Strength")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Intelligence,					"Attribute.Primary.Intelligence")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Dexterity,						"Attribute.Primary.Dexterity")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary_Vitality,						"Attribute.Primary.Vitality")
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_Health,							"Attribute.Main.Health")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_MaxHealth,						"Attribute.Main.MaxHealth")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_Mana,								"Attribute.Main.Mana")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_MaxMana,							"Attribute.Main.MaxMana")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_Stamina,							"Attribute.Main.Stamina")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_MaxStamina,						"Attribute.Main.MaxStamina")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_Poise,							"Attribute.Main.Poise")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_MaxPoise,							"Attribute.Main.MaxPoise")
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_MovementSpeed,					"Attribute.Main.MovementSpeed")
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_Armor,						"Attribute.Secondary.Armor")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_Evasion,						"Attribute.Secondary.Evasion")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_ArmorPenetration,			"Attribute.Secondary.ArmorPenetration")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_CriticalHitChance,			"Attribute.Secondary.CriticalHitChance")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_CriticalHitDamage,			"Attribute.Secondary.CriticalHitDamage")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_HealRegeneration,			"Attribute.Secondary.HealRegeneration")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_ManaRegeneration,			"Attribute.Secondary.ManaRegeneration")
}
