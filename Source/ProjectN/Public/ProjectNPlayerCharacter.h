// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectNCharacter_Base.h"
#include "Abilities/GameplayAbility.h"
#include "AbilitySystemInterface.h"
#include "ProjectN/ProjectNTypes.h"
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
	AProjectNPlayerCharacter(const FObjectInitializer& ObjectInitializer);
	virtual void PostInitializeComponents() override;

	bool ApplyGamePlayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext);

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCharacterData(const FCharacterData& NewCharacterData);

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const;
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data);

	void GiveAbilities();
	void ApplyStartupEffects();

	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;

	UPROPERTY(EditDefaultsOnly)
	UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent;

	UPROPERTY(Transient)
	UProjectN_AttributeSet* ProjectN_AttributeSet;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UFUNCTION()
	void OnRep_CharacterData();

	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);

	UPROPERTY(EditDefaultsOnly)
	class UProjectNCharacterDataAsset* CharacterDataAsset;
	
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
