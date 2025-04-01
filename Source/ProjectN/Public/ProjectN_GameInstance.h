// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "ProjectN_GameInstance.generated.h"

UCLASS()
class PROJECTN_API UProjectN_GameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	virtual void Init() override;

	UPROPERTY()
	int32 CurrentSaveSlotIndex = -1;

	UPROPERTY()
	FString CurrentSaveSlotName = FString();
};
