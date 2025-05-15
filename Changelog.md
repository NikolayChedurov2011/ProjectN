Remove addition HP and Mana by level.
New tag for block movement
Implement FProjectNGameplayEffectContext derived from FGameplayEffectContext to handle effect damage info (block, crit etc).
Implement custom UProjectN_AbilitySystemGlobals to use FProjectNGameplayEffectContext and enabled in DefaultGame.ini.
Iteration for physic damage calculation class and handle floating damage text depends on effect damage info.


TODO: 
Implement abilities
Implement overlap and damage from weapon
Setup physical calculation and magic
Remove evasion
Damage depends from attributes
UI for inventory and equipping
Target lock system and spawn projectiles to this direction
Animation system
Error  LogAbilitySystem FGameplayEffectSpec::GetMagnitude called for Data Damage.Physical on Def Default__GE_Damage_C when magnitude had not yet been set by caller.