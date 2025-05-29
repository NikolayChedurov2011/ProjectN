Mark classes for delete ("Mark for delete")

Massive items and inventory refactor, remove item static class and item instance. Use itemId, item type and data tables only

Now we can add bagID to inventory component to set default bag
Now we can "load" data for action bar to restore it (id, action bar index, entry type), and if this item have ability it will work

TODO: 

ROOTMOTION (With rootmotion we can run, walk or stop with attack or hit reaction and we no need modify movement speed I guess)
Abilities relationship

МБ каждое оружие может дать определённые доп способности, которые можно перенести на панель и использовать?

Setup tags to block other abilities
Stop movement when attack
Calc damage type defense and multiply
Setup physical calculation and magic
Remove evasion
Damage depends from attributes
UI for inventory and equipping
Target lock system and spawn projectiles to this direction
Animation system
Error  LogAbilitySystem FGameplayEffectSpec::GetMagnitude called for Data Damage.Physical on Def Default__GE_Damage_C when magnitude had not yet been set by caller.