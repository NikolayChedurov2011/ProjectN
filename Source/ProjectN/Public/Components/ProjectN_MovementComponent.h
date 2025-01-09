// N Chedurov All Rights Reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ProjectN_MovementComponent.generated.h"

struct FOnAttributeChangeData;
/**
 * 
 */
UCLASS()
class PROJECTN_API UProjectN_MovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
};
