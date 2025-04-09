// N Chedurov All Rights Reserved

#include "ProjectN_PlayerCharacter.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "ProjectN_GameInstance.h"
#include "ProjectN_GameplayTags.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/Input/ProjectN_InputComponent.h"
#include "ProjectN_PlayerState.h"
#include "AbilitySystem/ProjectN_AbilitySystemLibrary.h"
#include "AbilitySystem/Attribute/ProjectN_AttributeSet.h"
#include "Controllers/ProjectN_PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUD/ProjectN_HUD.h"
#include "UI/VievModel/MVVM_SaveSlot.h"

AProjectN_PlayerCharacter::AProjectN_PlayerCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
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
}

/*********************************
 * Combat interface Get function
 ********************************/
int32 AProjectN_PlayerCharacter::GetCharacterLevel_Implementation() const
{
	const AProjectN_PlayerState* ProjectNPlayerState = Cast<AProjectN_PlayerState>(GetPlayerState());
	if (ProjectNPlayerState)
	{
		return ProjectNPlayerState->GetCharacterLevel_Internal();
	}
	return 0;
}

/***********************
 * Character initialize
 ***********************/
void AProjectN_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for ability system component on the Server side. Bind for some attribute changes
	InitAbilityActorInfo();
	GiveStartupAbilitiesAndEffects();
}

void AProjectN_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for ability system component on  the Client side. Bind for some attribute changes. Init HUD overlay
	InitAbilityActorInfo();
}

void AProjectN_PlayerCharacter::InitAbilityActorInfo()
{
	AProjectN_PlayerState* ProjectN_PlayerState = GetPlayerState<AProjectN_PlayerState>();
	check(ProjectN_PlayerState)
	
	ProjectN_AbilitySystemComponent = Cast<UProjectN_AbilitySystemComponent>(ProjectN_PlayerState->GetAbilitySystemComponent());
	ProjectN_AttributeSet = ProjectN_PlayerState->GetAttributeSet();
	GetAbilitySystemComponent()->InitAbilityActorInfo(ProjectN_PlayerState, this);
	
	if (AProjectN_PlayerController* ProjectN_PlayerController = GetController<AProjectN_PlayerController>())
	{
		if (AProjectN_HUD* HUD = Cast<AProjectN_HUD>(ProjectN_PlayerController->GetHUD()))
		{
			HUD->InitOverlay(ProjectN_PlayerController, ProjectN_PlayerState, ProjectN_AbilitySystemComponent, ProjectN_AttributeSet);

			// At first apply saved attributes value
			if (!Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotName.IsEmpty())
			{
				const int32 Index = Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotIndex;
				const FString SlotName = Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotName;

				ApplyAttributesFromSave(SlotName, Index);
				ApplyPlayerInfoFromSave(SlotName, Index);
			}
		}
	}
	
	Super::InitAbilityActorInfo();
}

void AProjectN_PlayerCharacter::GiveStartupAbilitiesAndEffects()
{
	if (HasAuthority() && GetAbilitySystemComponent())
	{
		FGameplayEffectContextHandle EffectContext = GetAbilitySystemComponent()->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		// Activate base passive effects
		for (const TSubclassOf DefaultEffect : GetCharacterData().PassiveEffects)
		{
			Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->ApplyGamePlayEffectToSelf_Internal(DefaultEffect, EffectContext, 1.f);
		}

		for (const TSubclassOf DefaultAbility : GetCharacterData().DefaultAbilities)
		{
			Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->AddAbility(DefaultAbility);
		}
		
		for (const TSubclassOf DefaultAbility : GetCharacterData().PassiveAbilities)
		{
			Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->AddPassiveAbility(DefaultAbility);
		}

		//  Effect for bind dependency of health or other main attribute with their primary attributes
		Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->ApplyGamePlayEffectToSelf_Internal(GetCharacterData().SetPrimaryAttributeDefaultValues, EffectContext, 1.f);
		
		//  Effect for bind dependency of health or other main attribute with their primary attributes
		Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->ApplyGamePlayEffectToSelf_Internal(GetCharacterData().InitializeAttributeDependencies, EffectContext, 1.f);

		// Call to init health and other main attributes
		Cast<UProjectN_AbilitySystemComponent>(GetAbilitySystemComponent())->ApplyGamePlayEffectToSelf_Internal(GetCharacterData().InitHealthAndMana, EffectContext, 1.f);
	}
}

/***************************
 * Move and look functions
 ***************************/
void AProjectN_PlayerCharacter::Input_Move(const FInputActionValue& ActionValue)
{
	const FVector NoZVector(1.f, 1.f, 0.f);
	const FVector2d MovementVector = ActionValue.Get<FVector2d>();

	// Getting rotator
	const FRotator CameraRot = Cast<APlayerController>(Controller)->PlayerCameraManager->GetCameraRotation();
	
	// Get forward vector from rotator and normalize it
	FVector MovementForwardVector = CameraRot.Vector() * NoZVector;
	MovementForwardVector.Normalize();
	
	// Get right vector from rotator and normalize it
	FVector MovementRightVector = FRotationMatrix(CameraRot).GetScaledAxis(EAxis::Y) * NoZVector;
	MovementRightVector.Normalize();
	
	
	//const FRotator MovementRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);

	if(MovementVector.Y != 0.f)
	{
		//const FVector ForwardDirection = MovementRotation.RotateVector(FVector::ForwardVector);

		AddMovementInput(MovementForwardVector, MovementVector.Y);
	}

	if(MovementVector.X != 0.f)
	{
		//const FVector RightDirection = MovementRotation.RotateVector(FVector::RightVector);
		//const FVector RightDirection = FRotationMatrix(MovementRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(MovementRightVector, MovementVector.X);
	}
}

void AProjectN_PlayerCharacter::Input_Look(const FInputActionValue& ActionValue)
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

/*void AProjectN_PlayerCharacter::SaveNewSlot(UMVVM_SaveSlot* ViewModel) const
{
	/*ViewModel->Strength = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetStrength();
	ViewModel->Intelligence = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetIntelligence();
	ViewModel->Dexterity = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetDexterity();
	ViewModel->Vitality = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetVitality();#1#

	if (UGameplayStatics::DoesSaveGameExist(ViewModel->SlotName, ViewModel->SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(ViewModel->SlotName, ViewModel->SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CharacterSaveClass);
	UCharacter_Save* CharacterSave = Cast<UCharacter_Save>(SaveGameObject);
	
	CharacterSave->PlayerName = ViewModel->GetPlayerName();
	CharacterSave->SlotStatus = ESaveSlotStatus::Taken;
	
	/*CharacterSave->Strength = ViewModel->Strength;
	CharacterSave->Intelligence = ViewModel->Intelligence;
	CharacterSave->Dexterity = ViewModel->Dexterity;
	CharacterSave->Vitality = ViewModel->Vitality;#1#
	
	//CharacterSave->Level = Cast<AProjectN_PlayerState>(GetPlayerState())->GetCharacterLevel_Internal();
	//CharacterSave->XP = Cast<AProjectN_PlayerState>(GetPlayerState())->GetXP();
	//CharacterSave->AttributePoints = Cast<AProjectN_PlayerState>(GetPlayerState())->GetAttributePoints();

	UGameplayStatics::SaveGameToSlot(CharacterSave, ViewModel->SlotName, ViewModel->SlotIndex);
}*/

void AProjectN_PlayerCharacter::LoadGameSlot(UMVVM_SaveSlot* ViewModel) const
{
	Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotIndex = ViewModel->SlotIndex;
	Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotName = ViewModel->SlotName;
	
	ServerTravelToMap();
}

/*
void AProjectN_PlayerCharacter::SaveAttributes() const
{
	
}

void AProjectN_PlayerCharacter::SavePlayerInfo() const
{
	
}*/

void AProjectN_PlayerCharacter::ServerApplyAttributesFromSave_Implementation(const float Strength, const float Intelligence, const float Dexterity, const float Vitality) const
{
	UProjectN_AbilitySystemLibrary::SetPrimaryAttributesByCaller(this, GetAbilitySystemComponent(), Strength, Intelligence, Dexterity, Vitality);
}

void AProjectN_PlayerCharacter::ApplyAttributesFromSave(const FString& SlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGame = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	//	ServerApplyAttributesFromSave(Cast<UCharacter_Save>(SaveGame)->Strength, Cast<UCharacter_Save>(SaveGame)->Intelligence, Cast<UCharacter_Save>(SaveGame)->Dexterity, Cast<UCharacter_Save>(SaveGame)->Vitality);
	}
	else
	{
		SaveGame = UGameplayStatics::CreateSaveGameObject(CharacterSaveClass);
	//	ServerApplyAttributesFromSave(Cast<UCharacter_Save>(SaveGame)->Strength, Cast<UCharacter_Save>(SaveGame)->Intelligence, Cast<UCharacter_Save>(SaveGame)->Dexterity, Cast<UCharacter_Save>(SaveGame)->Vitality);
	}

	Cast<AProjectN_PlayerState>(GetPlayerState())->SetSkillTreePoints(Cast<UCharacter_Save>(SaveGame)->SkillTreePoints);
	Cast<AProjectN_PlayerState>(GetPlayerState())->SetSkillTreePointsInUse(Cast<UCharacter_Save>(SaveGame)->SkillTreePoints);
}

void AProjectN_PlayerCharacter::ApplyPlayerInfoFromSave(const FString& SlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGame = nullptr;
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
	}
	else
	{
		SaveGame = UGameplayStatics::CreateSaveGameObject(CharacterSaveClass);
	}

	Cast<AProjectN_PlayerState>(GetPlayerState())->SetLevel(Cast<UCharacter_Save>(SaveGame)->Level);
	Cast<AProjectN_PlayerState>(GetPlayerState())->SetXP(Cast<UCharacter_Save>(SaveGame)->XP);
}

void AProjectN_PlayerCharacter::ServerTravelToMap_Implementation() const
{
	UGameplayStatics::OpenLevel(this, "NewMap");
}

void AProjectN_PlayerCharacter::AddXP_Implementation(const int32 XPToAdd)
{
	Cast<AProjectN_PlayerState>(GetPlayerState())->AddToXP(XPToAdd);
}

void AProjectN_PlayerCharacter::AddToLevel_Implementation(const int32 LevelsToAdd)
{
	Cast<AProjectN_PlayerState>(GetPlayerState())->AddToLevel(LevelsToAdd);
}

void AProjectN_PlayerCharacter::AddToAttributePoints_Implementation(const int32 AttributePointsToAdd)
{
	Cast<AProjectN_PlayerState>(GetPlayerState())->AddToSkillTreePoints(AttributePointsToAdd);
	Cast<AProjectN_PlayerState>(GetPlayerState())->AddToSkillTreePointsInUse(AttributePointsToAdd);
}

int32 AProjectN_PlayerCharacter::GetXP_Implementation() const
{
	return Cast<AProjectN_PlayerState>(GetPlayerState())->GetXP();
}

int32 AProjectN_PlayerCharacter::GetAttributePointsReward_Implementation(const int32 InLevel) const
{
	return Cast<AProjectN_PlayerState>(GetPlayerState())->GetSkillTreePointsRewardForLevel(InLevel);
}

int32 AProjectN_PlayerCharacter::GetLevelByXP_Implementation(const int32 InXP)
{
	return Cast<AProjectN_PlayerState>(GetPlayerState())->GetLevelByXP(InXP);
}

void AProjectN_PlayerCharacter::LevelUP_Implementation()
{
	
}

/******************
* Console commands
*******************/
void AProjectN_PlayerCharacter::Console_AddXP(const float XPToAdd) const
{
	//ServerAddXP(XPToAdd);
	ProjectN_AbilitySystemComponent->ServerAddToAttributeByTag(ProjectNGameplayTags::Attribute_XP, XPToAdd);
}

void AProjectN_PlayerCharacter::ServerAddXP_Implementation(const float XPToAdd)
{	
	
}