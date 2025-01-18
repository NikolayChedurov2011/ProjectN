// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ProjectN_PlayerController.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class PROJECTN_API AProjectN_PlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AProjectN_PlayerController();

protected:
	virtual void BeginPlay() override;
};
