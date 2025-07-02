Finish for procedural character walk animation. Thoughts for the best animation solution..



TODO:



On destroy - destroy all spawned items



Check if inventory has free slot before any operation





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
We have damage ability. Inside need to set up "bUse Weapon Damage", "Physical Damage Multiplier", "AbilityID" - if use ability and "Required slot" - if "bUse Weapon Damage" is true.
This ability get all damage value itself.
Next from this ability we get damage projectile ability - from weapon socket or any other socket.

Damage ability has own on activate and logic to collect data from DT
Other abilities has own on activate and logic to collect data from DT



Next need ability for consumable or buff - different heal potion or buff attribute.
It will be instant potion for heal/mana and buff duration potion for all attributes. Its mean we will have 2 effect and an use only one ability
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////





maybe concept not "item weight" but the item that slowdown the player's movement" to remove weight?

Combo
Ability requirement
Load Items
Load Action slots
Warn if item was added or not

Левая рука - защитные способности, правая - атакующие

Try best modifiers for crit damage

Model for save/load data:
Character(Level/XP)
AbilityBook(AbilityID)
ActionBar(ActioSlotIndex/ItemID/Type)
Bags(BagID)
InventoryItem(ItemID/Type/Quantity/BagIndex/BagSlot)



ROOTMOTION (With rootmotion we can run, walk or stop with attack or hit reaction and we no need modify movement speed I guess)
Abilities relationship

МБ каждое оружие может дать определённые доп способности, которые можно перенести на панель и использовать?

Setup tags to block other abilities
Stop movement when attack
Target lock system and spawn projectiles to this direction
Animation system



!!!!!!!!!!!!
Error  LogAbilitySystem FGameplayEffectSpec::GetMagnitude called for Data Damage.Physical on Def Default\_\_GE\_Damage\_C when magnitude had not yet been set by caller.

