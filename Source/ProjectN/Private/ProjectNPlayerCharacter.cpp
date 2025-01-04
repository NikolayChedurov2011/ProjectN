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

AProjectNPlayerCharacter::AProjectNPlayerCharacter()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	SpringArm->SetupAttachment(GetRootComponent());
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 500.f, 0.f);
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	ProjectN_AbilitySystemComponent = CreateDefaultSubobject<UProjectN_AbilitySystemComponent>(TEXT("ProjectN Ability System Component"));
	ProjectN_AbilitySystemComponent->SetIsReplicated(true);
	ProjectN_AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	ProjectN_AttributeSet = CreateDefaultSubobject<UProjectN_AttributeSet>(TEXT("ProjectN Attribute Set"));
}

UAbilitySystemComponent* AProjectNPlayerCharacter::GetAbilitySystemComponent() const
{
	return ProjectN_AbilitySystemComponent;
}

void AProjectNPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AProjectNPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	ProjectN_AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	InitializeAttributes();
	GiveAbilities();
	ApplyStartupEffects();
}

void AProjectNPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	ProjectN_AbilitySystemComponent->InitAbilityActorInfo(this, this);

	InitializeAttributes();
}

void AProjectNPlayerCharacter::InitializeAttributes()
{
	if (GetLocalRole() == ROLE_Authority && ProjectN_AttributeSet && DefaultAttributeSetEffect)
	{
		FGameplayEffectContextHandle EffectContext = ProjectN_AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		ApplyGamePlayEffectToSelf(DefaultAttributeSetEffect, EffectContext);
	}
}

void AProjectNPlayerCharacter::GiveAbilities()
{
	if (HasAuthority() && ProjectN_AbilitySystemComponent)
	{
		for (auto DefaultAbility : DefaultAbilities)
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

		for (auto DefaultEffect : DefaultEffects)
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