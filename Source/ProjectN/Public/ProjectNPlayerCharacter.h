// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectNCharacter_Base.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "ProjectNPlayerCharacter.generated.h"

class UDataAsset_InputConfig;
class USpringArmComponent;
class UCameraComponent;
class UProjectN_AbilitySystemComponent;
class UProjectN_AttributeSet;
struct FInputActionValue;

class UGamePlayEffect;
class UGameplayAbility;

UCLASS()
class PROJECTN_API AProjectNPlayerCharacter : public AProjectNCharacter_Base, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectNPlayerCharacter();

	bool ApplyGamePlayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void InitializeAttributes();
	void GiveAbilities();
	void ApplyStartupEffects();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TSubclassOf<UGameplayEffect> DefaultEffectToSetAttribute;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayAbility>> DefaultAbilities;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "GAS")
	TArray<TSubclassOf<UGameplayEffect>> DefaultEffects;

	UPROPERTY(EditDefaultsOnly)
	UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent;

	UPROPERTY(Transient)
	UProjectN_AttributeSet* ProjectN_AttributeSet;
	
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
