Iteration for composite pattern.
Iteration for bag slots. Refactor item slots structure.
Remove broadcasts from inventory component, use PostReplicatedChange of Fast Array Serializer function. 

TODO: 

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
Error  LogAbilitySystem FGameplayEffectSpec::GetMagnitude called for Data Damage.Physical on Def Default__GE_Damage_C when magnitude had not yet been set by caller.