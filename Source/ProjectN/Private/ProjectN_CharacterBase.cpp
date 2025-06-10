// N Chedurov All Rights Reserved

#include "ProjectN_CharacterBase.h"

#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Components/CapsuleComponent.h"
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
	GetAbilitySystemComponent()->GetGameplayAttributeValueChangeDelegate(Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetMovementSpeedMultiplierAttribute()).AddUObject(this, &AProjectN_CharacterBase::OnMovementSpeedChanged);
}

void AProjectN_CharacterBase::OnMovementSpeedChanged(const FOnAttributeChangeData& Data) const
{
	IAvatarInfoInterface::Execute_UpdateMovementSpeedMultiplier(GetController(), Data.NewValue);
	GetCharacterMovement()->MaxWalkSpeed = IAvatarInfoInterface::Execute_GetAvatarMovementData(GetController()).MaxWalkSpeed * Data.NewValue;
}

/***********************
 * Character die
 ***********************/
void AProjectN_CharacterBase::Die()
{
	MulticastHandleDeath();
}

void AProjectN_CharacterBase::MulticastHandleDeath_Implementation()
{
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->SetEnableGravity(true);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	GetMesh()->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

/*************************
*  Avatar Actor Interface
**************************/
FVector AProjectN_CharacterBase::GetAvatarSocketLocation_Implementation(const FName SocketName) const
{
	return GetMesh()->GetSocketLocation(SocketName);
}
