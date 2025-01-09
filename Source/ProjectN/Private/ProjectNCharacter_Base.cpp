// N Chedurov All Rights Reserved

#include "ProjectNCharacter_Base.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "DataAssets/ProjectN_CharacterDataAsset.h"
#include "Components/ProjectN_MovementComponent.h"
#include "Net/UnrealNetwork.h"

AProjectNCharacter_Base::AProjectNCharacter_Base(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UProjectN_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
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
}

void AProjectNCharacter_Base::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
}

void AProjectNCharacter_Base::InitAbilityActorInfo()
{
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(GetAttributeSet()->GetMaxMovementSpeedAttribute()).AddUObject(this, &AProjectNCharacter_Base::OnMaxMovementSpeedChanged);
}
/*
 *
 */

/*
 *** Give startup gameplay abilities and effects + ApplyGamePlayEffectToSelf function
 */
void AProjectNCharacter_Base::GiveAbilities()
{
	if (HasAuthority() && GetAbilitySystemComponent())
	{
		for (const TSubclassOf DefaultAbility : CharacterData.Abilities)
		{
			GetAbilitySystemComponent()->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void AProjectNCharacter_Base::ApplyStartupEffects()
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

bool AProjectNCharacter_Base::ApplyGamePlayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const
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