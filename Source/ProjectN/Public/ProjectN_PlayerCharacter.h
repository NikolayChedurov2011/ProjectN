// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_CharacterBase.h"
#include "DataAssets/ProjectN_CharacterDataAsset.h"
#include "ProjectN_PlayerCharacter.generated.h"

class UProjectN_InputConfig;
class USpringArmComponent;
class UCameraComponent;
class AProjectN_PlayerCameraManager;
class UCharacter_Save;
class UMVVM_SaveSlot;
class USaveGame;

struct FInputActionValue;

UCLASS(Abstract)
class PROJECTN_API AProjectN_PlayerCharacter : public AProjectN_CharacterBase
{
	GENERATED_BODY()

public:
	AProjectN_PlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	void Save(UMVVM_SaveSlot* ViewModel) const;

	UFUNCTION(Server, Reliable)
	void ServerApplyPrimaryAttributeFromSave(const float Strength, const float Intelligence, const float Dexterity, const float Vitality) const;
	void ApplyPrimaryAttributeFromSave(const FString& SlotName, const int32 SlotIndex) const;

	UFUNCTION(Server, Reliable)
	void ServerTravelToMap() const;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE FCharacterData& GetCharacterData() const { return CharacterDataAsset->CharacterData; }

	/*
	*  Combat Interface
	*/
	virtual int32 GetCharacterLevel() const override;

protected:
	virtual void InitAbilityActorInfo() override;
	virtual void GiveStartupAbilitiesAndEffects() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<USaveGame> CharacterSaveClass;

	// Set default data from Default
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectN_CharacterDataAsset> CharacterDataAsset = nullptr;

private:
	
#pragma region Components
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<USpringArmComponent> SpringArm = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	TObjectPtr<UCameraComponent> CameraComponent = nullptr;
	
#pragma endregion

#pragma region Inputs
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CharacterData", meta=(AllowPrivateAccess=true))
	UProjectN_InputConfig* InputConfigDataAsset = nullptr;

	void Input_Move(const FInputActionValue& ActionValue);
	void Input_Look(const FInputActionValue& ActionValue);	
#pragma endregion
};
