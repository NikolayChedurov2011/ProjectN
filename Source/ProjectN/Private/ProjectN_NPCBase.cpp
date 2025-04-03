// N Chedurov All Rights Reserved


#include "ProjectN_NPCBase.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"

AProjectN_NPCBase::AProjectN_NPCBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProjectN_AbilitySystemComponent = CreateDefaultSubobject<UProjectN_AbilitySystemComponent>("AbilitySystem_Component");
	ProjectN_AbilitySystemComponent->SetIsReplicated(true);
	ProjectN_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	ProjectN_AttributeSet = CreateDefaultSubobject<UProjectN_AttributeSet>("AttributeSet");
}

void AProjectN_NPCBase::BeginPlay()
{
	Super::BeginPlay();
	
	GetAbilitySystemComponent()->InitAbilityActorInfo(this, this);

	InitAbilityActorInfo();
	GiveStartupAbilitiesAndEffects();
}

void AProjectN_NPCBase::GiveStartupAbilitiesAndEffects()
{
	if (HasAuthority() && GetAbilitySystemComponent())
	{
		CharacterDataAsset->GetNPCData(NPCRarity);

		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);
		
		/*for (const TSubclassOf DefaultAbility : GetCharacterData().PassiveAbilities)
		{
			Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->AddPassiveAbility(DefaultAbility);
		}*/

		// Set primary attributes based on level and rarity
		Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->ApplyGamePlayEffectToSelf_Internal(CharacterDataAsset->GetNPCData(NPCRarity).PrimaryAttributes, EffectContext, GetNPCLevel());
		
		//  Effect for bind dependency of health or other main attribute with their primary attributes
		Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->ApplyGamePlayEffectToSelf_Internal(CharacterDataAsset->AttributesDependency, EffectContext, 1.f);

		// Call to init health and other main attributes
		Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->ApplyGamePlayEffectToSelf_Internal(CharacterDataAsset->InitializeAttributes, EffectContext, 1.f);

		for (const TSubclassOf DefaultAbility : CharacterDataAsset->GetNPCData(NPCRarity).NPCAbilities)
		{
			Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->AddAbility(DefaultAbility);
		}
	}
}

void AProjectN_NPCBase::SetNPCLevel(const int32 NewNPCLevel)
{
	NPCLevel = NewNPCLevel;
}
