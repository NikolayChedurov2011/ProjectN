// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_CharacterBase.h"
#include "DataAssets/ProjectN_NPCDataAsset.h"
#include "Interfaces/NPCInterface.h"
#include "ProjectN_NPCBase.generated.h"

UCLASS()
class PROJECTN_API AProjectN_NPCBase : public AProjectN_CharacterBase, public INPCInterface
{
	GENERATED_BODY()

public:
	AProjectN_NPCBase(const FObjectInitializer& ObjectInitializer);
	
	/*************************
	*  Avatar Actor Interface
	**************************/
	virtual int32 GetCharacterLevel_Implementation() const override { return NPCLevel; }

	/********************
	 *  Combat Interface
	 ********************/
	virtual float GetNPCRewardXP_Implementation() const override;
	virtual ENPCRarity GetNPCRarity_Implementation() const override;
	virtual int32 GetNPCLevel_Implementation() const override;

	void HitReactTagChanged(const FGameplayTag CallbackTag, const int32 NewCount);
	virtual void Die() override;

	UPROPERTY(BlueprintReadOnly, Category="Combat")
	bool bHitReacting = false;
	
protected:
	virtual void BeginPlay() override;
	virtual void GiveStartupAbilitiesAndEffects() override;

	// Set default data from Default
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectN_NPCDataAsset> NPCDataAsset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn = true))
	int32 NPCLevel = 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(ExposeOnSpawn = true))
	ENPCRarity NPCRarity = ENPCRarity::Common;
};
