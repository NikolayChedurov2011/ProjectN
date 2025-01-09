// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectNCharacter_Base.h"
#include "ProjectN_PlayerCharacter.generated.h"

class UDataAsset_InputConfig;
class USpringArmComponent;
class UCameraComponent;

struct FInputActionValue;

UCLASS()
class PROJECTN_API AProjectN_PlayerCharacter : public AProjectNCharacter_Base
{
	GENERATED_BODY()

public:
	AProjectN_PlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

protected:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void InitAbilityActorInfo() override;

	UFUNCTION(Server, Reliable)
	void OnCharacterInitAbilityEnd();

private:
	
#pragma region Components
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	USpringArmComponent* SpringArm = nullptr;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Camera", meta=(AllowPrivateAccess=true))
	UCameraComponent* CameraComponent = nullptr;
	
#pragma endregion

#pragma region Inputs
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="CharacterData", meta=(AllowPrivateAccess=true))
	UDataAsset_InputConfig* InputConfigDataAsset = nullptr;

	void Input_Move(const FInputActionValue& ActionValue);
	void Input_Look(const FInputActionValue& ActionValue);
	
#pragma endregion
};
