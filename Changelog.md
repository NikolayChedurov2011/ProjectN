Implement widget component and it spawn widget with damage number.
Now damage effect use only calculation class, without set value but effect apply set by caller value and calculation class catch it.
Remove evasion and all magic armor attribute.
Work for physical calculation class.

TODO: 
Setup physical calculation and magic
Remove evasion
Damage depends from attributes
UI for inventory and equipping
Target lock system and spawn projectiles to this direction
Animation system
Error  LogAbilitySystem FGameplayEffectSpec::GetMagnitude called for Data Damage.Physical on Def Default__GE_Damage_C when magnitude had not yet been set by caller.