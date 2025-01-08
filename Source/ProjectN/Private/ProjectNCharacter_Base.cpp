// N Chedurov All Rights Reserved

#include "ProjectNCharacter_Base.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "DataAssets/ProjectNCharacterDataAsset.h"
#include "Net/UnrealNetwork.h"

#include "GameFramework/CharacterMovementComponent.h"

AProjectNCharacter_Base::AProjectNCharacter_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	ProjectN_AttributeSet = CreateDefaultSubobject<UProjectN_AttributeSet>(TEXT("ProjectN Attribute Set"));

	ProjectN_AbilitySystemComponent = CreateDefaultSubobject<UProjectN_AbilitySystemComponent>(TEXT("ProjectN Ability System Component"));
	ProjectN_AbilitySystemComponent->SetIsReplicated(true);
	ProjectN_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	ProjectN_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &AProjectNCharacter_Base::OnMaxMovementSpeedChanged);
}

void AProjectNCharacter_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectNCharacter_Base, CharacterData);
}

/*
 *** Main Get and Set functions
 */
UAbilitySystemComponent* AProjectNCharacter_Base::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

FCharacterData AProjectNCharacter_Base::GetCharacterData() const
{
	return  CharacterData;
}

void AProjectNCharacter_Base::SetCharacterData(const FCharacterData& NewCharacterData)
{
	CharacterData = NewCharacterData;
	InitFromCharacterData(CharacterData);
}
/*
 *
 */

/*
 *** Character initialize
 */
//First in initialization order
void AProjectNCharacter_Base::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}
//Second in initialization order
void AProjectNCharacter_Base::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectNCharacter_Base::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ProjectN_AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	GiveAbilities();
	ApplyStartupEffects();
}

void AProjectNCharacter_Base::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ProjectN_AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
/*
 *
 */

/*
 *** Give startup gameplay abilities and effects + ApplyGamePlayEffectToSelf function
 */
void AProjectNCharacter_Base::GiveAbilities()
{
	if (HasAuthority() && ProjectN_AbilitySystemComponent)
	{
		for (auto DefaultAbility : CharacterData.Abilities)
		{
			ProjectN_AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void AProjectNCharacter_Base::ApplyStartupEffects()
{
	if (HasAuthority() && ProjectN_AbilitySystemComponent)
	{
		FGameplayEffectContextHandle EffectContext = ProjectN_AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (auto DefaultEffect : CharacterData.Effects)
		{
			ApplyGamePlayEffectToSelf(DefaultEffect, EffectContext);
		}
	}
}

bool AProjectNCharacter_Base::ApplyGamePlayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const
{
	if (!Effect.Get())
	{
		return false;
	}

	FGameplayEffectSpecHandle SpecHandle = ProjectN_AbilitySystemComponent->MakeOutgoingSpec(Effect, 1,InEffectContext);
	if (SpecHandle.IsValid())
	{
		FActiveGameplayEffectHandle ActiveGameplayEffectHandle = ProjectN_AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}
	
	return true;
}
/*
 *
 */

void AProjectNCharacter_Base::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication)
{
	
}

void AProjectNCharacter_Base::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

void AProjectNCharacter_Base::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}
