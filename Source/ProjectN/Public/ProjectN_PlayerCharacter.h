// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_CharacterBase.h"
#include "DataAssets/ProjectN_CharacterDataAsset.h"
#include "Interfaces/PlayerInterface.h"
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
class PROJECTN_API AProjectN_PlayerCharacter : public AProjectN_CharacterBase, public IPlayerInterface
{
	GENERATED_BODY()

public:
	AProjectN_PlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	//void SaveNewSlot(UMVVM_SaveSlot* ViewModel) const;
	void LoadGameSlot(UMVVM_SaveSlot* ViewModel) const;

	UFUNCTION(Server, Reliable)
	void ServerApplyAttributesFromSave(const float Strength, const float Intelligence, const float Dexterity, const float Vitality) const;
	void ApplyAttributesFromSave(const FString& SlotName, const int32 SlotIndex) const;
	void ApplyPlayerInfoFromSave(const FString& SlotName, const int32 SlotIndex) const;

	UFUNCTION(Server, Reliable)
	void ServerTravelToMap() const;
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE FCharacterData& GetCharacterData() const { return CharacterDataAsset->CharacterData; }

	/*************************
	*  Avatar Actor Interface
	**************************/
	virtual int32 GetCharacterLevel_Implementation() const override;

	/********************
	*  Player Interface
	*********************/
	virtual void AddXP_Implementation(const int32 XPToAdd) override;
	virtual void AddToLevel_Implementation(const int32 LevelsToAdd) override;
	virtual void AddToAttributePoints_Implementation(const int32 AttributePointsToAdd) override;
	virtual int32 GetXP_Implementation() const override;
	virtual int32 GetAttributePointsReward_Implementation(const int32 InLevel) const override;
	virtual int32 GetLevelByXP_Implementation(const int32 InXP) override;
	virtual void LevelUP_Implementation() override;

	UFUNCTION(Exec)
	void Console_AddXP(const float XPToAdd) const;
	UFUNCTION(Server, Reliable)
	void ServerAddXP(const float XPToAdd);

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
