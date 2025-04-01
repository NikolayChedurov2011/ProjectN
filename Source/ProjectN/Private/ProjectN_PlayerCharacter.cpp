// N Chedurov All Rights Reserved

#include "ProjectN_PlayerCharacter.h"

#include "ProjectN_GameInstance.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DataAssets/InputConfig/DataAsset_InputConfig.h"
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

/*
 *** Character initialize
 */
void AProjectN_PlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Init ability actor info for ability system component on the Server side. Bind for some attribute changes
	InitAbilityActorInfo();
}

void AProjectN_PlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Init ability actor info for ability system component on  the Client side. Bind for some attribute changes. Init HUD overlay
	InitAbilityActorInfo();
	
	//Send RPS from Client to Server when it's ready
	//OnCharacterInitAbilityEnd();
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
		}
	}

	if (HasAuthority())
	{
		// At first apply saved attributes value
		if (!Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotName.IsEmpty())
		{
			const int32 Index = Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotIndex;
			const FString SlotName = Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotName;

			ApplyPrimaryAttributeFromSave(SlotName, Index);
		}

		GiveStartupAbilities();
		ApplyStartupEffects();
	}
	
	Super::InitAbilityActorInfo();
}

/*
void AProjectN_PlayerCharacter::OnCharacterInitAbilityEnd_Implementation()
{

}
*/

/*
 *
 */

/*
 *** Move and look functions
 */

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

void AProjectN_PlayerCharacter::OnActionPressed(FGameplayTag InTag)
{
	
}

void AProjectN_PlayerCharacter::OnActionReleased(FGameplayTag InTag)
{
	
}

void AProjectN_PlayerCharacter::OnActionHeld(FGameplayTag InTag)
{
	
}

/*
 *
 */

void AProjectN_PlayerCharacter::Save(UMVVM_SaveSlot* ViewModel) const
{
	ViewModel->Strength = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetStrength();
	ViewModel->Intelligence = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetIntelligence();
	ViewModel->Dexterity = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetDexterity();
	ViewModel->Vitality = Cast<UProjectN_AttributeSet>(GetAttributeSet())->GetVitality();

	if (UGameplayStatics::DoesSaveGameExist(ViewModel->GetSlotName(), ViewModel->SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(ViewModel->GetSlotName(), ViewModel->SlotIndex);
	}
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CharacterSaveClass);
	UCharacter_Save* CharacterSave = Cast<UCharacter_Save>(SaveGameObject);
	
	CharacterSave->PlayerName = ViewModel->GetPlayerName();
	CharacterSave->SlotStatus = ESaveSlotStatus::Taken;
	CharacterSave->Strength = ViewModel->Strength;
	CharacterSave->Intelligence = ViewModel->Intelligence;
	CharacterSave->Dexterity = ViewModel->Dexterity;
	CharacterSave->Vitality = ViewModel->Vitality;

	UGameplayStatics::SaveGameToSlot(CharacterSave, ViewModel->GetSlotName(), ViewModel->SlotIndex);

	Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotIndex = ViewModel->SlotIndex;
	Cast<UProjectN_GameInstance>(GetGameInstance())->CurrentSaveSlotName = ViewModel->GetSlotName();

	ServerTravelToMap();
}

void AProjectN_PlayerCharacter::ServerApplyPrimaryAttributeFromSave_Implementation(const float Strength, const float Intelligence, const float Dexterity, const float Vitality) const
{
	UProjectN_AbilitySystemLibrary::OverridePrimaryAttributes(this, GetAbilitySystemComponent(), Strength, Intelligence, Dexterity, Vitality);
}

void AProjectN_PlayerCharacter::ApplyPrimaryAttributeFromSave(const FString& SlotName, const int32 SlotIndex) const
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		USaveGame* SaveGame = UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex);
		ServerApplyPrimaryAttributeFromSave(Cast<UCharacter_Save>(SaveGame)->Strength, Cast<UCharacter_Save>(SaveGame)->Intelligence, Cast<UCharacter_Save>(SaveGame)->Dexterity, Cast<UCharacter_Save>(SaveGame)->Vitality);
	}
	else
	{
		USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(CharacterSaveClass);
		ServerApplyPrimaryAttributeFromSave(Cast<UCharacter_Save>(SaveGameObject)->Strength, Cast<UCharacter_Save>(SaveGameObject)->Intelligence, Cast<UCharacter_Save>(SaveGameObject)->Dexterity, Cast<UCharacter_Save>(SaveGameObject)->Vitality);
	}
}

void AProjectN_PlayerCharacter::ServerTravelToMap_Implementation() const
{
	UGameplayStatics::OpenLevel(this, "NewMap");
}
