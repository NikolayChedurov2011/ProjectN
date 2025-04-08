// N Chedurov All Rights Reserved

#include "ProjectN_CharacterBase.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Components/ProjectN_MovementComponent.h"

AProjectN_CharacterBase::AProjectN_CharacterBase(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UProjectN_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	GetMesh()->bReceivesDecals = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 550.f, 0.f);
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
}

/******************************
 * Main Get and Set functions
 ******************************/
UAbilitySystemComponent* AProjectN_CharacterBase::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

/***********************
 * Character initialize
 ***********************/
void AProjectN_CharacterBase::GiveStartupAbilitiesAndEffects()
{
	
}

void AProjectN_CharacterBase::InitAbilityActorInfo()
{
	ProjectN_AbilitySystemComponent->AbilityActorInfoSet();
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetMovementSpeedAttribute()).AddUObject(this, &AProjectN_CharacterBase::OnMaxMovementSpeedChanged);
}

void AProjectN_CharacterBase::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}