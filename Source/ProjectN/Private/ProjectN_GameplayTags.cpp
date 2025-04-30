// N Chedurov All Rights Reserved


#include "ProjectN_GameplayTags.h"

namespace ProjectNGameplayTags
{
	/**** Input Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Input,							"Input")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move,					"InputTag.Move")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look,					"InputTag.Look")
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Esc,					"InputTag.Esc")
	UE_DEFINE_GAMEPLAY_TAG(Input_Interaction,				"Input.Interaction")
	UE_DEFINE_GAMEPLAY_TAG(Input_Inventory,					"Input.Inventory")
	UE_DEFINE_GAMEPLAY_TAG(Input_RMB,						"Input.RMB")
	UE_DEFINE_GAMEPLAY_TAG(Input_LMB,						"Input.LMB")
	UE_DEFINE_GAMEPLAY_TAG(Input_Sprint,					"Input.Sprint")
	UE_DEFINE_GAMEPLAY_TAG(Input_Alt,						"Input.Alt")
	UE_DEFINE_GAMEPLAY_TAG(Input_Crouch,					"Input.Crouch")
	UE_DEFINE_GAMEPLAY_TAG(Input_Jump,						"Input.Jump")
	UE_DEFINE_GAMEPLAY_TAG(Input_Map,						"Input.Map")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot1,						"Input.Slot1")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot2,						"Input.Slot2")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot3,						"Input.Slot3")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot4,						"Input.Slot4")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot5,						"Input.Slot5")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot6,						"Input.Slot6")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot7,						"Input.Slot7")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot8,						"Input.Slot8")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot9,						"Input.Slot9")
	UE_DEFINE_GAMEPLAY_TAG(Input_Slot0,						"Input.Slot0")
	
	/**** Inventory Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(InventoryTag_Equip,				"InventoryTag.Equip")
	UE_DEFINE_GAMEPLAY_TAG(InventoryTag_UnEquip,			"InventoryTag.UnEquip")
	UE_DEFINE_GAMEPLAY_TAG(InventoryTag_Drop,				"InventoryTag.Drop")

	/**** Inventory Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon,							"Item.Weapon")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Type,					"Item.Weapon.Type")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Type_Dagger,				"Item.Weapon.Type.Dagger")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Type_Sword,				"Item.Weapon.Type.Sword")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Type_GreatSword,			"Item.Weapon.Type.GreatSword")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Type_Bow,				"Item.Weapon.Type.Bow")
	
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Damage,					"Item.Weapon.Damage")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Damage_Min,				"Item.Weapon.Damage.Min")
	UE_DEFINE_GAMEPLAY_TAG(Item_Weapon_Damage_Max,				"Item.Weapon.Damage.Max")
	
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Slot,						"Equipment.Slot")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Slot_Main,					"Equipment.Slot.Main")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Slot_Auxiliary,			"Equipment.Slot.Auxiliary")
	UE_DEFINE_GAMEPLAY_TAG(Equipment_Slot_TwoHanded,			"Equipment.Slot.TwoHanded")

	/**** Message Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Message,					"Message")
	UE_DEFINE_GAMEPLAY_TAG(Message_Potion,			"Message.Potion")
	UE_DEFINE_GAMEPLAY_TAG(Message_Damage,			"Message.Damage")

	/**** Attributes Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Primary,								"Attribute.Primary")
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
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_MaxMovementSpeed,					"Attribute.Main.MaxMovementSpeed")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_EquipmentWeight,					"Attribute.Main.EquipmentWeight")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Main_MaxCarryingCapacity,				"Attribute.Main.MaxCarryingCapacity")
	
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_Armor,						"Attribute.Secondary.Armor")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_Evasion,						"Attribute.Secondary.Evasion")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_BlockChance,					"Attribute.Secondary.BlockChance")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_ArmorPenetration,			"Attribute.Secondary.ArmorPenetration")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_CriticalHitChance,			"Attribute.Secondary.CriticalHitChance")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_CriticalHitDamage,			"Attribute.Secondary.CriticalHitDamage")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_HealRegeneration,			"Attribute.Secondary.HealRegeneration")
	UE_DEFINE_GAMEPLAY_TAG(Attribute_Secondary_ManaRegeneration,			"Attribute.Secondary.ManaRegeneration")

	UE_DEFINE_GAMEPLAY_TAG(Attribute_XP,									"Attribute.XP")

	/**** Event Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Event_Montage_SpawnEffect,			"Event.Montage.SpawnEffect")

	/**** Menu Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Menu_SaveSlot,						"Menu.SaveSlot")

	/**** Progression Tags ****/
	UE_DEFINE_GAMEPLAY_TAG(Progression_AttributePoint,			"Progression.AttributePoint")
}
