// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "ProjectN_CharacterBase.h"
#include "DataAssets/ProjectN_NPCDataAsset.h"
#include "ProjectN_NPCBase.generated.h"

UCLASS()
class PROJECTN_API AProjectN_NPCBase : public AProjectN_CharacterBase
{
	GENERATED_BODY()

public:
	AProjectN_NPCBase(const FObjectInitializer& ObjectInitializer);
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetNPCLevel() const { return NPCLevel; }
	FORCEINLINE void SetNPCLevel(int32 NewNPCLevel);
	
	/*
	 *  Combat Interface
	 */
	virtual int32 GetCharacterLevel() const override { return NPCLevel; }
	
protected:
	virtual void BeginPlay() override;
	virtual void GiveStartupAbilitiesAndEffects() override;

	// Set default data from Default
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UProjectN_NPCDataAsset> CharacterDataAsset = nullptr;

	UPROPERTY(EditAnywhere)
	int32 NPCLevel = 1;

	UPROPERTY(EditAnywhere)
	ENPCRarity NPCRarity = ENPCRarity::Common;
};
