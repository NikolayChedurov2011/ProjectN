// N Chedurov All Rights Reserved

#include "ProjectN_CharacterBase.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "DataAssets/ProjectN_CharacterDataAsset.h"
#include "Components/ProjectN_MovementComponent.h"
#include "Net/UnrealNetwork.h"

AProjectN_CharacterBase::AProjectN_CharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UProjectN_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 550.f, 0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

void AProjectN_CharacterBase::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectN_CharacterBase, CharacterData);
}

/*
 *** Main Get and Set functions
 */
UAbilitySystemComponent* AProjectN_CharacterBase::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

FCharacterData AProjectN_CharacterBase::GetCharacterData() const
{
	return  CharacterData;
}

void AProjectN_CharacterBase::SetCharacterData(const FCharacterData& NewCharacterData)
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
void AProjectN_CharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}
//Second in initialization order
void AProjectN_CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectN_CharacterBase::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
}

void AProjectN_CharacterBase::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AProjectN_CharacterBase::InitAbilityActorInfo()
{
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetMaxMovementSpeedAttribute()).AddUObject(this, &AProjectN_CharacterBase::OnMaxMovementSpeedChanged);
}
/*
 *
 */

/*
 *** Give startup gameplay abilities and effects + ApplyGamePlayEffectToSelf function
 */
void AProjectN_CharacterBase::GiveAbilities()
{
	if (HasAuthority() && GetAbilitySystemComponent())
	{
		for (const TSubclassOf DefaultAbility : CharacterData.Abilities)
		{
			GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void AProjectN_CharacterBase::ApplyStartupEffects()
{
	if (HasAuthority() && GetAbilitySystemComponent())
	{
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		for (const TSubclassOf DefaultEffect : CharacterData.Effects)
		{
			ApplyGamePlayEffectToSelf(DefaultEffect, EffectContext);
		}
	}
}

bool AProjectN_CharacterBase::ApplyGamePlayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const
{
	if (!Effect.Get())
	{
		return false;
	}

	const FGameplayEffectSpecHandle SpecHandle = GetAbilitySystemComponent()->MakeOutgoingSpec(Effect, 1,InEffectContext);
	if (SpecHandle.IsValid())
	{
		const FActiveGameplayEffectHandle ActiveGameplayEffectHandle = GetAbilitySystemComponent()->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());

		return ActiveGameplayEffectHandle.WasSuccessfullyApplied();
	}
	
	return true;
}
/*
 *
 */

void AProjectN_CharacterBase::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication)
{
	
}

void AProjectN_CharacterBase::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

void AProjectN_CharacterBase::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}