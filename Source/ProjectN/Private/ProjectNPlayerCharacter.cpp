// N Chedurov All Rights Reserved

#include "ProjectNPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DataAssets/InputConfig/DataAsset_InputConfig.h"
#include "Components/Input/ProjectNInputComponent.h"
#include "ProjectNGameplayTags.h"

#include "AbilitySystemComponent.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Components/ProjectN_MovementComponent.h"
#include "DataAssets/ProjectNCharacterDataAsset.h"

#include "Net/UnrealNetwork.h"

AProjectNPlayerCharacter::AProjectNPlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UProjectN_MovementComponent>(ACharacter::CharacterMovementComponentName))
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 600.f;
	SpringArm->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	ProjectN_AttributeSet = CreateDefaultSubobject<UProjectN_AttributeSet>(TEXT("ProjectN Attribute Set"));
	
	ProjectN_AbilitySystemComponent = CreateDefaultSubobject<UProjectN_AbilitySystemComponent>(TEXT("ProjectN Ability System Component"));
	ProjectN_AbilitySystemComponent->SetIsReplicated(true);
	ProjectN_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	ProjectN_AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(ProjectN_AttributeSet->GetMaxMovementSpeedAttribute()).AddUObject(this, &AProjectNPlayerCharacter::OnMaxMovementSpeedChanged);
}

void AProjectNPlayerCharacter::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AProjectNPlayerCharacter, CharacterData);
}

/*
 *** Character initialize
 */

//First in initialization order
void AProjectNPlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (IsValid(CharacterDataAsset))
	{
		SetCharacterData(CharacterDataAsset->CharacterData);
	}
}
//Second in initialization order
void AProjectNPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectNPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ProjectN_AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	GiveAbilities();
	ApplyStartupEffects();
}

void AProjectNPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ProjectN_AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
/*
 *
 */

/*
 *** Main Get and Set functions
 */
UAbilitySystemComponent* AProjectNPlayerCharacter::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

FCharacterData AProjectNPlayerCharacter::GetCharacterData() const
{
	return  CharacterData;
}

void AProjectNPlayerCharacter::SetCharacterData(const FCharacterData& NewCharacterData)
{
	CharacterData = NewCharacterData;
	InitFromCharacterData(CharacterData);
}

void AProjectNPlayerCharacter::OnRep_CharacterData()
{
	InitFromCharacterData(CharacterData, true);
}

void AProjectNPlayerCharacter::InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication)
{
	
}
/*
 *
 */

/*
 *** Give startup gameplay abilities and effects + ApplyGamePlayEffectToSelf function
 */
void AProjectNPlayerCharacter::GiveAbilities()
{
	if (HasAuthority() && ProjectN_AbilitySystemComponent)
	{
		for (auto DefaultAbility : CharacterData.Abilities)
		{
			ProjectN_AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbility));
		}
	}
}

void AProjectNPlayerCharacter::ApplyStartupEffects()
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

bool AProjectNPlayerCharacter::ApplyGamePlayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext)
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

/*
 *** Setup InputComponent + implement move and look functions
 */
void AProjectNPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	checkf(InputConfigDataAsset, TEXT("Forgot to assign valid data asset"));
	
	const ULocalPlayer* LocalPLayer = GetController<APlayerController>()->GetLocalPlayer();
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPLayer);

	check(EnhancedInputSubsystem);

	EnhancedInputSubsystem->AddMappingContext(InputConfigDataAsset->DefaultMappingContext, 0);
	UProjectNInputComponent* ProjectNInputComponent = CastChecked<UProjectNInputComponent>(PlayerInputComponent);

	ProjectNInputComponent->BindNativeInputAction(InputConfigDataAsset, ProjectNGameplayTags::InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move);
	ProjectNInputComponent->BindNativeInputAction(InputConfigDataAsset, ProjectNGameplayTags::InputTag_Look, ETriggerEvent::Triggered, this, &ThisClass::Input_Look);
}

void AProjectNPlayerCharacter::Input_Move(const FInputActionValue& ActionValue)
{
	const FVector2d MovementVector = ActionValue.Get<FVector2d>();
	const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if(MovementVector.Y != 0.f)
	{
		const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(ForwardDirection, MovementVector.Y);
	}

	if(MovementVector.X != 0.f)
	{
		const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);

		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AProjectNPlayerCharacter::Input_Look(const FInputActionValue& ActionValue)
{
	const FVector2d LookAxisVector = ActionValue.Get<FVector2d>();

	if(LookAxisVector.X != 0.f)
	{
		AddControllerYawInput(LookAxisVector.X);
	}

	if(LookAxisVector.Y != 0.f)
	{
		AddControllerPitchInput(LookAxisVector.Y);
	}
}
/*
 *
 */
void AProjectNPlayerCharacter::OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}
