// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ProjectN/ProjectNTypes.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "Interfaces/CombatInterface.h"
#include "ProjectN_CharacterBase.generated.h"

class UGamePlayEffect;
class UGameplayAbility;
class UAttributeSet;

UCLASS(Abstract)
class PROJECTN_API AProjectN_CharacterBase : public ACharacter, public IAbilitySystemInterface, public ICombatInterface
{
	GENERATED_BODY()

public:
	AProjectN_CharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UAttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCharacterData(const FCharacterData& NewCharacterData);

	/*
	 *  Combat Interface
	 */
	virtual int32 GetCharacterLevel() const override;

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);
	virtual void InitAbilityActorInfo();

	void GiveStartupAbilitiesAndEffects();

	UPROPERTY(Transient)
	TObjectPtr<UAttributeSet> ProjectN_AttributeSet = nullptr;

	UPROPERTY()
	TObjectPtr<UProjectN_AbilitySystemComponent> ProjectN_AbilitySystemComponent = nullptr;

	// Set default data from Default
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UProjectN_CharacterDataAsset> CharacterDataAsset = nullptr;

	// Save data from CharacterDataAsset when character init
	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UFUNCTION()
	void OnRep_CharacterData();

	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const;
};
