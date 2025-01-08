// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "GameFramework/Character.h"
#include "ProjectN/ProjectNTypes.h"
#include "ProjectNCharacter_Base.generated.h"

class UProjectN_AbilitySystemComponent;
class UProjectN_AttributeSet;
class UGamePlayEffect;
class UGameplayAbility;

UCLASS(Abstract)
class PROJECTN_API AProjectNCharacter_Base : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AProjectNCharacter_Base(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void BeginPlay() override;
	virtual void PossessedBy(AController* NewController) override;
	virtual void OnRep_PlayerState() override;
	virtual void PostInitializeComponents() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void InitFromCharacterData(const FCharacterData& InCharacterData, bool bFromReplication = false);
	bool ApplyGamePlayEffectToSelf(TSubclassOf<UGameplayEffect> Effect, const FGameplayEffectContextHandle& InEffectContext) const;

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE FCharacterData GetCharacterData() const;

	UFUNCTION(BlueprintCallable)
	FORCEINLINE void SetCharacterData(const FCharacterData& NewCharacterData);

	void GiveAbilities();
	void ApplyStartupEffects();
	void OnMaxMovementSpeedChanged(const FOnAttributeChangeData& Data) const;
	
	UPROPERTY(Transient)
	UProjectN_AttributeSet* ProjectN_AttributeSet;

	UPROPERTY(EditDefaultsOnly)
	UProjectN_AbilitySystemComponent* ProjectN_AbilitySystemComponent;
	
	UPROPERTY(EditDefaultsOnly)
	class UProjectN_CharacterDataAsset* CharacterDataAsset;

	UPROPERTY(ReplicatedUsing = OnRep_CharacterData)
	FCharacterData CharacterData;

	UFUNCTION()
	void OnRep_CharacterData();
};
