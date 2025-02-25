// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ProjectN/ProjectNTypes.h"
#include "AbilitySystem/ProjectN_AbilitySystemComponent.h"
#include "ProjectN_CharacterBase.generated.h"

class UGamePlayEffect;
class UGameplayAbility;
class UProjectN_AttributeSet;

UCLASS(Abstract)
class PROJECTN_API AProjectN_CharacterBase : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectN_CharacterBase(const FObjectInitializer& ObjectInitializer);
	
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	UProjectN_AttributeSet* GetAttributeSet() const { return ProjectN_AttributeSet; }
	
	UFUNCTION(BlueprintCallable)
	FORCEINLINE FCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCharacterData(const FCharacterData& NewCharacterData);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostInitializeComponents() override;

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);
	bool ApplyGamePlayEffectToSelf(const TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const;
	virtual void InitAbilityActorInfo();

	void GiveAbilities();
	void ApplyStartupEffects();

	UPROPERTY(Transient)
	TObjectPtr<UProjectN_AttributeSet> ProjectN_AttributeSet = nullptr;

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
